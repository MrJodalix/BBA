#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import subprocess
import json
import argparse
import os
import math

try:
    import ctypes
    kernel32 = ctypes.windll.kernel32
    kernel32.SetConsoleMode(kernel32.GetStdHandle(-11), 7) # erlaubt Farben in Windows Konsole
except AttributeError:
    pass

class ExpectationNotMetError(Exception):
    pass

TERMCOLOR_RED = "\033[31m"
TERMCOLOR_GREEN = "\033[32m"
TERMCOLOR_BOLD = "\033[1m"
TERMCOLOR_RESET = "\033[0m"

def poor_mans_pgm_parser(pgmdata):
    if (not pgmdata):
        raise ExpectationNotMetError('Keine Daten erhalten.')
    channels = None
    if (pgmdata.startswith('P2')):
        channels = 1
    elif (pgmdata.startswith('P3')):
        channels = 3
    else:
        raise ExpectationNotMetError('Kann nur Portable Graymap (P2) und Portable Pixmap (P3) im ASCII Format verarbeiten. Die ersten Bytes dieser Daten sind: "%s"'%(pgmdata[:min(10,len(pgmdata))]))
    lines = pgmdata.split('\n')
    lines = [l for l in lines if not l.startswith('#')]
    pgmdata = '\n'.join(lines)
    tokens = pgmdata.split()
    def interpret_tokens(tokens):
        if (len(tokens) < 4):
            raise ExpectationNotMetError(
                'Zu wenig Felder in PXM Kopfdaten "%s".'%(str(tokens))
            )
        typeinfo = tokens[0]
        try:
            width = int(tokens[1])
            height = int(tokens[2])
            intensity = int(tokens[3])
        except ValueError as verr:
            raise ExpectationNotMetError(
                'Erwartete Zahl in PXM Kopfdaten, bei der Umwandlung fand ich jedoch "%s".'%(' '.join(verr.args))
            )
        _datasize = width*height*channels
        try:
            pixels = [int(t) for t in tokens[4:4+_datasize]]
        except ValueError as verr:
            raise ExpectationNotMetError(
                'Erwartete Zahl in PXM Bilddaten, bei der Umwandlung fand ich jedoch "%s".'%(' '.join(verr.args))
            )
        if (len(tokens) > 4+_datasize):
            return pixels+interpret_tokens(tokens[4+_datasize:])
        else:
            return pixels
    return interpret_tokens(tokens)

def compare_values(reference, delivered, tolerance, reference_title = ""):
    if (reference_title):
        reference_title = " "+reference_title
    if (len(reference) != len(delivered)):
        raise ExpectationNotMetError(
            'Referenz{} hat andere Anzahl von Werten. Erwartete {}, bekam {}.'.format(
                reference_title,
                len(reference),
                len(delivered)
            )
        )
    adiff = [abs(r-d) for r,d in zip(reference, delivered)]
    if (any([v > tolerance for v in adiff])):
        if (len(adiff) > 25):
            print('Auflösung zu groß: Unterschiede werden nicht auf der Konsole gezeigt.')
            # TODO: bilder als dateien schreiben
        else:
            sigificant_digits = -math.floor(math.log(tolerance if tolerance else 1, 10))
            def floats2str(fs):
                return '['+', '.join([('{:+0.%sf}'%(sigificant_digits)).format(f) for f in fs])+']'
            print('SOLL:', floats2str(reference))
            print('IST: ', floats2str(delivered))
            print('DIFF:', floats2str(adiff))
        raise ExpectationNotMetError('Referenz{} hat andere Werte.'.format(reference_title))

def escape_spaces(path):
    return os.sep.join(['"%s"'%(p) if " " in p else p for p in path.split(os.sep)])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='BBA Testwerkzeug',
    formatter_class=argparse.RawDescriptionHelpFormatter,
    # https://stackoverflow.com/questions/50021282/python-argparse-how-can-i-add-text-to-the-default-help-message
    epilog='''\
    Testdefinition ist eine JSON Liste von Objekten.
    
    Dies sind alle unterstützten Schlüssel für ein solches Objekt:
    {
        "title": "Testtitel",
        "command": "{DUT} {IMGDIR}bild.pgm --printpnm", # Komanndozeilenargumente, Variablen werden ersetzt
        "expect": {
            "optional": true, # Test darf Fehlschlagen, Gesamtergebnis ist nicht beeinträchtigt
            "bytes": "", # Vergleich gegen String
            "image": "{IMGDIR}bild.pgm", # Vergleich gegen Referenzbild
            "pixels": [0, 1, 2 ,3], # Ausgabe wird als PXM Bild interpretiert und die einzelnen Intensitätswerte numerisch verglichen
            "eval": [0, 1, 2 ,3], # Ausgabe wird als Python Datum interpretiert und als Liste numerischer Werte verglichen
            "tolerance": 1, # Toleranz für numerische Vergleiche
        },
        "ask": "War das Ergebnis ok?" # explizite Rückfrage an den Tester für nicht-automatisierbare Tests
    }

    Ein wie oben definierter Test führt das zu testende Programm (DUT) aus. Es wird erwartet, dass DUT ausschließlich auf die Standardausgabe schreibt (nicht in Dateien). Die gesamte Ausgabe wird als ein Datensatz verarbeitet. Bei mehrfacher Angabe von --printpnm werden die Pixel aneinandergehängt.
    
    Es darf jeweils nur ein Erwartungswert (bytes^image^pixels^eval) angegeben werden. pixels und image erwarten Portable Anymap Daten (P2 oder P3).
''')
    parser.add_argument('test', type=open, help='Testdefinition')
    parser.add_argument('--dut', type=str, help='Zu testendes Programm', default=os.path.join('.','bba')) # TODO: type=executable file
    parser.add_argument('--expand', action='store_true', help='Befehle vor dem Anzeigen mit konkreten Daten füllen.')
    parser.add_argument('--keepgoing', action='store_true', help='Bei Fehlern mit nächstem Test weitermachen.')
    parser.add_argument('--imgdir', type=str, help='Ordner zu Referenzbildern', default='images') # TODO: type=directory
    parser.add_argument('--tolerance', type=float, help='Toleranz bei numerischen Vergleichen')
    parser.add_argument('--timeout', type=float, help='Maximale Ausführungszeit', default=None)
    parser.add_argument('--batch', action='store_true', help='Ignoriere nicht-vollautomatischen Testtyp "ask".')
    args = parser.parse_args()
    tests = json.load(args.test)
    if (not args.imgdir.endswith(os.sep)):
        args.imgdir = os.path.join(args.imgdir,'') # fügt einen slash passend zum os hinzu
    environment = {'DUT': args.dut, 'IMGDIR': args.imgdir}
    allok = True
    if (not tests):
        raise ValueError('Keine Tests definiert.')
    for test in tests:
        try:
            if ("blocking" in test):
                del test["blocking"]
                if (args.batch):
                    print('Vollautomatischer Modus aktiv: Lasse blockierenden Test "{}" aus.'.format(test["title"]))
                    continue
            optional = False
            if ("optional" in test):
                optional = True
                del test["optional"]
            print('Teste nun "{}"…'.format(test["title"]))
            del test["title"]
            command_template = test["command"]
            del test["command"]
            command = command_template.format(**{k:escape_spaces(v) for k,v in environment.items()})
            print('Befehl ist "{}{}{}"'.format(TERMCOLOR_BOLD, command if args.expand else command_template, TERMCOLOR_RESET))
            env = os.environ.copy()
            env["OPENCV_LOG_LEVEL"] = "ERROR" # compensate for https://www.reddit.com/r/opencv/comments/ea2fuk/
            p = subprocess.run(command, stdout=subprocess.PIPE, shell=True, check=True, encoding='utf-8', timeout=args.timeout, env=env)
            if ("ask" in test):
                if (args.batch):
                    print('Ignoriere Test mit Rückfrage.')
                else:
                    answer = None
                    while not answer:
                        answer = input(test["ask"]+" (y/n) ")
                    if (answer not in ["y","j"]):
                        raise ExpectationNotMetError('Erwartete positive Antwort.')
                del test["ask"]
            if ("expect" in test):
                expect = test["expect"]
                tolerance = 0.0
                if (args.tolerance):
                    if ("tolerance" in expect):
                        print("Überschreibe in Test definierte Toleranz {} mit {}…".format(expect["tolerance"], args.tolerance))
                        del expect["tolerance"]
                    tolerance = args.tolerance
                elif ("tolerance" in expect):
                    tolerance = expect["tolerance"]
                    del expect["tolerance"]
                if ("bytes" in expect):
                    if (p.stdout != expect["bytes"]):
                        raise ExpectationNotMetError('Erwartete "{}", bekam "{}".'.format(expect["bytes"], p.stdout))
                    del expect["bytes"]
                if ("image" in expect):
                    reference = poor_mans_pgm_parser(open(expect["image"].format(**environment)).read())
                    delivered = poor_mans_pgm_parser(p.stdout)
                    compare_values(reference, delivered, tolerance, expect["image"])
                    del expect["image"]
                if ("eval" in expect):
                    delivered = None
                    try:
                        delivered = eval(p.stdout.replace(";", ",").replace("\n"," "))
                    except TypeError:
                        raise ExpectationNotMetError('Eingabe "{}" konnte nicht interpretiert werden.'.format(p.stdout))
                    except SyntaxError:
                        raise ExpectationNotMetError('Eingabe "{}" konnte nicht interpretiert werden.'.format(p.stdout))
                    if delivered is not None:
                        reference = expect["eval"]
                        if (not isinstance(reference, list)):
                            reference = [reference]
                            delivered = [delivered]
                        compare_values(reference, delivered, tolerance)
                    del expect["eval"]
                if ("pixels" in expect):
                    reference = expect["pixels"]
                    delivered = poor_mans_pgm_parser(p.stdout)
                    compare_values(reference, delivered, tolerance)
                    del expect["pixels"]
                if (expect):
                    raise NotImplementedError('Nicht implementierter erwarteter Wert "{}"'.format(", ".join(expect.keys())))
                del test["expect"]
            if (test):
                raise NotImplementedError('Nicht implementierter Test-Fall "{}"'.format(", ".join(test.keys())))
            print("{}OK{}".format(TERMCOLOR_GREEN,TERMCOLOR_RESET))
        except (subprocess.CalledProcessError, ExpectationNotMetError) as e:
            if (optional):
                print("Optionaler Test {}nicht erfolgreich{}: {}".format(TERMCOLOR_RED, TERMCOLOR_RESET, e))
            else:
                print("{}FEHLER:{} {}".format(TERMCOLOR_RED, TERMCOLOR_RESET, e))
                allok = False
                if (not args.keepgoing):
                    break
        except UnicodeDecodeError as ude:
            # TODO: diesen catch näher an open bzw. im poor_mans_pgm_parser haben
            print("{}Nicht Implementiert:{} Binärdaten werden nicht unterstützt. Bitte nur mit ASCII-Bildern arbeiten.".format(TERMCOLOR_RED, TERMCOLOR_RESET))
            allok = False
            if (not args.keepgoing):
                raise
        print("") # leere Zeile für optische Trennung zwischen Tests
    if (allok):
        print("{}{}Alles klar gut und so und von dem Test IHM sein Ende.{}".format(TERMCOLOR_BOLD,TERMCOLOR_GREEN,TERMCOLOR_RESET))
    else:
        print("{}{}ONEIM! Es snid Fehlre aufgetretne.{}".format(TERMCOLOR_BOLD,TERMCOLOR_RED,TERMCOLOR_RESET))
