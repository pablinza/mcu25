# Microcontroladores PIC/AVR pablinza@me.com
Programacion de Microcontroladores en lenguaje C con el compilador oficial de Microchip __XC8__ <br />
Carpeta con las practicas desarroladas durante el curso intensivo de Microcontroladores ELT-336 2025 de la UEB <br />
El software __MPLABX__ disponible en la pagina del fabricante Microchip [ --> Click](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/MPLABX-v6.20-windows-installer.exe?authuser=0) <br />
El compilador __XC8__ puedes descargalo utilizando este enlace [ --> Click](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/xc8-v2.50-full-install-windows-x64-installer.exe?authuser=0) <br />
Para cargar el firmware al microcontrolador necesitaras un programador ICSP, como alternativa se utiliza el software __SimulIDE__ [ -->Click](https://simulide.com/p/) a efector de verificar el funcionamiento <br />

Cada carpeta del proyecto MPLABX tiene el nombre precedido por el numero de actividad, en su estructura encontrara los del programa principal con nombre __main.c__ y librerias de uso local, una vez compilado el codigo se genera el firmware del programa __.hex__ en la carpeta dist/default/production. <br />

## Lista de practicas desarrolladas con Microcontrolador
### - ST1 - Multiplexor de 2 Canales con datos de 4-bit [MUX2S4B]
<p align="center">
  <img src="/images/st1_p887.png"></img>
</p>
Este primer circuito muestra la implementacion de un multiplexor simple de dos canales para datos de 4-bit, la teoria y diseño de este circuito se describe mejor en el siguiente enlace a mi blog. la descripcion del modulo Verilog se efectuara a nivel del comportamiento, para las pruebas de funcionamiento utilizaremos conmutadores (switches) a cada entrada y diodos led en las salidas. Puedes ver el video de la implementacion.

### - ST3 - Multiplexor de 2 Canales con datos de 4-bit [MUX2S4B]
<p align="center">
  <img src="/images/st3_p887.png"></img>
</p>
Este primer circuito muestra la implementacion de un multiplexor simple de dos canales para datos de 4-bit, la teoria y diseño de este circuito se describe mejor en el siguiente enlace a mi blog. la descripcion del modulo Verilog se efectuara a nivel del comportamiento, para las pruebas de funcionamiento utilizaremos conmutadores (switches) a cada entrada y diodos led en las salidas. Puedes ver el video de la implementacion.

### - ST5 - Sistema de control ADC y Pantalla LCD [MUX2S4B]
Con base al esquema de circuito MCU, se programaran las siguientes tareas: <br />
1. Destello de led RE2 con 10% de nivel activo <br />
2. Lectura del sensor conectado al canal ANS0 <br />
3. Enviar por el puerto serie el dato del sensor a escala 0-100 <br />
4. Muestra el valor del sensor en la pantalla LCD y el valor de umbral para alerta <br />
5. Lectura del encoder SW para ajuste de umbral en la pantalla LCD <br />
6. Aviso de alerta en LCD, si el valor del sensor supera al umbral ajustado <br />
<p align="right">
  <img src="/images/st5_p887.png"></img>
</p>
Con base al esquema de circuito MCU, se programaran las siguientes tareas: <br />
1. Destello de led RE2 con 10% de nivel activo <br />
2. Lectura del sensor conectado al canal ANS0 <br />
3. Enviar por el puerto serie el dato del sensor a escala 0-100 <br />
4. Muestra el valor del sensor en la pantalla LCD y el valor de umbral para alerta <br />
5. Lectura del encoder SW para ajuste de umbral en la pantalla LCD <br />
6. Aviso de alerta en LCD, si el valor del sensor supera al umbral ajustado <br />
<br />
Autor: Pablo Zarate, puedes contactarme a pablinza@me.com / pablinzte@gmail.com.  <br />
Visita mi Blog  [Blog de Contenidos](https://pablinza.blogspot.com/). <br />
Visita mi Canal [Blog de Contenidos](http://www.youtube.com/@pablozarate7524). <br />
Santa Cruz - Bolivia 
<br clear="left"/>

