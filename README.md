
![Mario](test/Mario%20pixeli.jpg)

🍄🍄🍄🍄🍄🍄🍄🍄🍄🍄🍄🍄🍄🍄🍄
1. incarcaTexturaInRAM(...)
Ce face: Aceasta este funcția responsabilă cu încărcarea imaginilor (sprite-urilor) de pe hard disk direct în memoria RAM a calculatorului.

Cum funcționează: Citește fișierul imagine de pe disc folosind readimagefile, îi află dimensiunea exactă cu imagesize, rezervă dinamic spațiu în memorie cu malloc și copiază imaginea în buffer-ul creat cu getimage.

De ce e utilă: Încărcarea se face o singură dată la pornirea jocului, evitând ca jocul să se blocheze sau să aibă lag în timpul mișcării din cauză că ar citi fișierele imagini de pe disc cadru cu cadru.

2. pregatesteImaginile()
Ce face: Pregătește toate elementele vizuale ale jocului înainte ca acesta să pornească efectiv.

Cum funcționează: Apelează funcția de încărcare în RAM descrisă mai sus (incarcaTexturaInRAM) pentru fiecare element în parte (Mario, pământ, nori, cărămizi, blocuri secrete, inamici Goomba, prințesă, Bowser, steag și țevi), stabilindu-le dimensiunile în pixeli. La final, curăță ecranul cu cleardevice().

3. initializeazaInamici()
Ce face: Scanează harta jocului (matricea mijloc) după caracterele 'g' și plasează inamicii Goomba exact la pozițiile lor inițiale.

Cum funcționează: Parcurge matricea nivelului, iar când găsește un 'g', calculează poziția în pixeli în funcție de coloană și rând, setează o viteză inițială de mișcare spre stânga (vitezaX = -3), îi activează viața (activ = true) și înlocuiește acel 'g' din hartă cu un spațiu liber ('.') pentru ca zona să poată fi randată corect.

4. getTile(int pixX, int pixY)
Ce face: Face conversia din coordonate grafice (pixeli de pe ecran) în coordonate logice (rândurile și coloanele matricii hărții).

Cum funcționează: Împarte poziția în pixeli la dimensiunea unui bloc (DIMENSIUNE_BLOC, care este 40). Verifică dacă personajul a ieșit în afara hărții și returnează caracterul ASCII corespunzător din matricea de cer, mijloc sau jos (de exemplu, un bloc solid sau aer).

5. esteSolid(char bloc)
Ce face: Verifică dacă un anumit element din hartă este o barieră fizică prin care personajul nu poate trece.

Cum funcționează: Compară caracterul primit ca parametru cu literele care reprezintă obiecte solide (cum ar fi cărămida 'b', blocul secret 'S', țevile 't', pământul 'p', blocul invizibil 'i' sau Bowser 'B'). Returnează true dacă este solid sau false dacă este doar aer/spațiu liber.

6. seCiocneste(int x, int y)
Ce face: Gestionează coliziunile de tip AABB (coliziuni geometrice simple) pentru Mario.

Cum funcționează: Verifică simultan cele 4 colțuri ale sprite-ului lui Mario (stânga-sus, dreapta-sus, stânga-jos, dreapta-jos) apelând funcția getTile. Dacă vreunul dintre colțuri atinge un bloc solid, funcția returnează true (adică există o coliziune și Mario nu poate trece prin perete sau podea).

7. deseneazaForma(char tip, int x, int y)
Ce face: Desenează efectiv imaginea potrivită pe ecran în funcție de litera găsită în matricea hărții.

Cum funcționează: Folosește funcția grafică putimage. Dacă întâlnește un 'p' afișează textura de pământ, dacă întâlnește un 'b' afișează cărămida, pentru 't' pune țeava, iar pentru caracterele goale (. sau ii) nu desenează nimic.

8. deseneazaEcranul(int cameraPixX)
Ce face: Randează întregul nivel vizual pe ecran ținând cont de deplasarea camerei (scrolling).

Cum funcționează: Parcurge pe rând toate cele trei secțiuni ale hărții (cerul, zona din mijloc și solul de jos) și calculează poziția fiecărui bloc scăzând poziția camerei (cameraPixX). Apoi apelează funcția deseneazaForma pentru a desena blocurile vizibile pe ecran.

9. main()
Ce face: Este funcția principală care pornește programul și conține motorul propriu-zis al jocului (bucla infinită de rulare).

Cum funcționează:

Inițializează fereastra grafică de 800x400 pixeli cu dublu buffer (initwindow).

Pregătește imaginile și plasează inamicii pe hartă.

Intră în bucla principală (while (inJoc)), unde rulează în mod repetat la fiecare 30 de milisecunde:

Citește comenzile tastaturii (GetAsyncKeyState pentru tastele A, D, W/Space, X) ca să mute personajul.

Calculează fizica săriturii și gravitația.

Verifică coliziunile cu inamicii Goomba (dacă Mario cade de sus pe un Goomba, îl elimină și primește puncte; dacă se lovește lateral, pierde vieți).

Actualizează poziția camerei în funcție de unde se află Mario.

Șterge ecranul și redesenează cadrul curent (hartă, inamici, Mario și interfața HUD cu scorul și viețile).

Închide fereastra grafică la ieșirea din joc cu closegraph().
