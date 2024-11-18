# Tietoliikenteen sovellusprojekti syksy 2024

Projektin tavoitteena oli tehdä nRF5340-kehitysalustalle ohjelma, joka ottaa dataa kiihtyvyysanturilta ja tekoälyn avulla päättelee siitä anturin asennon ja lähettää tämän langattomasti IoT-laitteelle, joka välittää tiedon eteenpäin tietokantaan. Tietokannasta tiedot pystyy lukemaan HTTP-rajapinnasta.

Lisäksi kurssilla opeteltiin scrum- ja kanban-metodien käyttöä projektin yhteydessä

Projekti suoritettiin OAMK:n tietotekniikan koulutusohjelman toisen vuosikurssin syksyllä.

## Ryhmän jäsenet

[@Nornalite](https://github.com/Nornalite/)

[@baarnuo](https://github.com/baarnuo/)

## Projektin vaiheet

#### Arkkitehtuurikuva
![Arkkitehtuurikuva](readme_images/Arkkitehtuurikuva_2.png?raw=true)

#### Hieno laitteistomme
![Anturi, nRF5340DK, Raspberry Pi ja tiedonsiirtoyhteys](readme_images/setup.jpg?raw=true)

### nRF 5340DK
[luki kiihtyvyysanturia ja siirsi datan BLE-yhteydellä, C käytössä]

### Raspberry Pi
[välikäsi datan siirrossa nettiin, Python käytössä]

### Linux-serveri ja MySQL-tietokanta
[hyödynsimme koululta saatua Linux-serveriä linuxin perusteiden opetteluun, sekä asetimme serverille MySQL-tietokantamme]

### Vau, koneoppimista
[k-means-algoritmi]

### Muut taidot
[viikkopalaverit, hyödynsimme kanban-taulua, scrum jne.]