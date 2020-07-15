# OS1
<i><b>Opis koda</b></i><br>
<li>Preemptive multitasking kernel with time sharing for x86 CPUs written in C++ and assembly.</li>

<i><b>Bitne ispravke u odnosu na verz 2</b></i>
<li>Uocena neispravnost rada semafora, tickAllSemaphores refactor</li>
<li>Uocena greska kod deklaracije element_for_return u svim queue klasama(dodato el=0;)</li>
<br>

<i><b>Nacin testiranja koda</b></i>
<li>Pokreni VMware i Eclipse, setuj BCC kompajler.</li>
<li>U test gde zelis da pokrenes, kopiraj sve header, src i javni test fajlove. Fajlovi ne treba da 
budu particionisani na foldere, svi treba da budu zajedno unutar jednog fajla (meni se zvao mod).</li>
<li>Build, desni klik na fajl, BCC -> Arguments input ->pa npr. postavi: 20 10 2, pa opet build za svaki slucaj, pa BCC->Run program.</li> 
<li>Sa esc iskljucujes program, obavezno pokreni vise puta unutar cmd-a (Komanda za pokretanje unutar cmd-a:mod.exe 30 15 2). 
Argumenti u komandnoj liniji mogu biti proizvoljni.</li>

<br><br>
<p align="center">
  <img src="/os_slika.png">
</p>