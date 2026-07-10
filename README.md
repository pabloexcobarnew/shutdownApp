# Shutdown.exe

Utilitar simplu de oprire a calculatorului cu un click, pentru **Windows**.
La dublu-click cere confirmare **Yes/No**, iar la *Yes* execută `shutdown /s /t 0`
(oprire imediată). Nu are fereastră de consolă și are o icoană verde de „power".

## Fișierul

Executabilul compilat este deja în repo — **nu se instalează nimic**, e portabil:

```
output/Shutdown.exe
```

## Rulare

Dublu-click pe `output\Shutdown.exe` → apare dialogul de confirmare → **Yes** oprește
calculatorul, **No** anulează. Butonul implicit este *No*, ca să nu se închidă din greșeală.

## Scurtătură pe Desktop („link")

1. Mută `Shutdown.exe` într-un loc fix (ex. `C:\Tools\Shutdown.exe`), ca linkul să nu
   se strice dacă ștergi folderul.
2. Click-dreapta pe fișier → **Send to → Desktop (create shortcut)**.
   (Windows 11: click-dreapta → *Show more options* → *Send to* → *Desktop*.)
3. Redenumește scurtătura „Shutdown". Are deja icoana de power.

Opțional:
- **Taskbar / Start:** click-dreapta pe exe → *Pin to taskbar* / *Pin to Start*.
- **Tastă rapidă:** click-dreapta pe scurtătură → *Properties* → câmpul *Shortcut key* →
  apasă o combinație (ex. `Ctrl + Alt + S`).

## Prima rulare — avertisment normal

Fiind un `.exe` nesemnat, Windows poate afișa o dată **„Windows protected your PC"
(SmartScreen)**: apasă **More info → Run anyway**.
Dacă ai descărcat repo-ul ca **ZIP** de pe GitHub, deblochează fișierul:
click-dreapta pe exe → *Properties* → bifează *Unblock* → *OK*.

## Recompilare din sursă (opțional)

Pe Linux cu `mingw-w64`:

```bash
python3 make_icon.py
x86_64-w64-mingw32-windres resource.rc -O coff -o resource.o
x86_64-w64-mingw32-gcc -O2 -Wall Shutdown.c resource.o -o output/Shutdown.exe -mwindows -s
```

## Atenție

`shutdown /s /t 0` oprește imediat, **fără delay** — salvează-ți munca înainte.
Pentru a anula o oprire pornită din greșeală (dacă ai pus un delay), rulează `shutdown /a`.
