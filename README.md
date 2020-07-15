# OS1
Preemptive multitasking kernel with time sharing for x86 CPUs written in C++ and assembly.

<b>Bitne ispravke u odnosu na verz 2</b>
<li>
	<ul>Uocena neispravnost rada semafora, tickAllSemaphores refactor</ul>
	<ul>Uocena greska kod deklaracije element_for_return u svim queue klasama(dodato el=0;)</ul>
</li><br>

<i>Nacin testiranja koda</i>
<li>
	<ul>Pokreni VMware i eclipse, setuj BCC kompajler
	<ul>U test gde zelis da pokrenes, kopiraj sve header, src i javni test fajlove. Fajlovi ne treba da 
	budu particionisani na foldere, svi treba da budu zajedno unutar jednog fajla (meni se zvao mod)
	<ul> Build, desni klik na fajl, BCC -> Arguments input ->pa npr. postavi [20 10 2], pa opet build 
	za svaki slucaj, pa BCC->Run program. 
	<ul> Sa esc iskljucujes program, obavezno pokreni vise puta unutar cmd (Komanda za pokretanje unutar cmd-a:mod.exe 30 15 2). Argumenti u komandnoj liniji mogu biti proizvoljni
</li>
