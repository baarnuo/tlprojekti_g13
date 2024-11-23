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
Luimme Nordicin kehitysalustalla kiihtyvyysanturilta ADC-kirjaston avulla suuntadataa, ja hyödynsimme alustan Bluetooth Low Energy -yhteyttä anturidatan eteenpäin lähetykseen. Aloituspisteinä toimivat opettajan tarjoama adc-lukuohjelma ja Nordic Semiconductorin Bluetooth Low Energy Fundamentals -kurssin esimerkit.

### Raspberry Pi
BLE:n yli lähetettävä anturidata luettiin Raspberry Pi 3:lle bleak-kirjaston avulla, ja lähetettiin mysql-kirjaston avulla eteenpäin Linux-serverillemme asennettuun tietokantaan.

### Linux-serveri ja MySQL-tietokanta
Saimme koululta allokoidut henkilökohtaiset Linux-serverit, joita käytimme Linuxin opetteluun (ja sitä tukevien lisätehtävien tekoon) sekä datan säilövän tietokantamme sijaintina. 

### Vau, koneoppimista
[k-means-algoritmi tulossa joskus]

### Muut taidot
Kurssilla opetettiin scrum-metodologiaa, ja pidimme säännöllisiä ryhmä- ja tiimipalavereja. Tehtävien seuraamisessa apuna oli GitHubin projektiosion kanban-taulu.