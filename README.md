# Tietoliikenteen sovellusprojekti syksy 2024

Projektin tavoitteena oli tehdä nRF5340-kehitysalustalle ohjelma, joka ottaa dataa kiihtyvyysanturilta, tekoälyn avulla päättelee siitä anturin asennon ja lähettää asennon langattomasti IoT-laitteelle. IoT-laite välittää tiedon eteenpäin tietokantaan. Tietokannasta tiedot pystyy lukemaan HTTP-rajapinnasta.

Lisäksi kurssilla opeteltiin scrum- ja kanban-metodien käyttöä projektin yhteydessä.

Projekti suoritettiin OAMK:n tietotekniikan koulutusohjelman toisen vuosikurssin syksyllä.

## Ryhmän jäsenet

[@Nornalite](https://github.com/Nornalite/)

[@baarnuo](https://github.com/baarnuo/)

## Projektin vaiheet

#### Kuva: Järjestelmän arkkitehtuuri
![Arkkitehtuurikuva](readme_images/Arkkitehtuurikuva_2.png?raw=true)

#### Kuva: Hieno laitteistomme - Raspberry Pi (vihertävä laatikko vasemmalla), kiihtyvyysanturi (punaisessa laatikossa), nRF 5340 -alusta (sinertävä levy alaoikealla), mittausdatan vastaanottoa Raspberry Pi:lle (ruudulla vasemmalla), tietokannan datan lukua HTTP-rajapinnasta (ruudulla oikealla)
![Anturi, nRF5340DK, Raspberry Pi ja tiedonsiirtoyhteys](readme_images/setup.jpg?raw=true)

### nRF 5340DK
Luimme Nordicin kehitysalustalla kiihtyvyysanturilta ADC-kirjaston avulla suuntadataa, ja hyödynsimme alustan Bluetooth Low Energy -yhteyttä anturidatan eteenpäin lähetykseen. Loimme alustalle kiihtyvyysanturipalvelun, jota laite mainostaa ympäröiville BLE-laitteille. Ulkoinen laite voi muodostaa yhteyden alustaamme tunnistettuaan sen palvelun UUID-numeron perusteella ja tilata mittaustiedot. Aina mittauksen suoritettuaan alustamme lähettää mittaustulokset sisältävän ilmoituksen tilaajalle.

Aloituspisteinä toimivat opettajan tarjoama adc-lukuohjelma ja Nordic Semiconductorin Bluetooth Low Energy Fundamentals -kurssin esimerkit.

### Koneoppiminen
Opetimme ja toteutimme kaksi koneoppimisalgoritmia suuntatiedon laskemiseen.

Ensisijaisena algoritmina toimi k-keskiarvoklusterointi, jossa arvotut datakeskittymien keskipisteet hakeutuvat kierros kierrokselta kohti lähimpiä datapisteitä. Algoritmin luomisen ja koulutuksen jälkeen toimme lasketut keskipisteet .h-tiedostoon, josta käytimme niitä luetun datan lähimmän keskipisteen eli suunnan määritykseen.

#### Kuva: K-keskiarvoklusterointialgoritmi laskennan ensimmäisen kierroksen jälkeen
![K-keskiarvoklusterointialgoritmi laskennan ensimmäisen kierroksen jälkeen](readme_images/readings_with_centers.png?raw=true)

Lisätehtävänä opetin Tensorflow-kirjaston avulla yksinkertaisen neuroverkon lajittelemaan datapisteitä. Hyviä tuloksia antoi herätteiden luokitteluun tarkoitetun Catergorical Crossentropy -häviöfunktion ja suurista korjausliikkeistä aloittavan, vähitellen tarkentuvan Exponential Decay -optimisaattorin käyttö. Opetuksen jälkeen toin painokertoimet ja bias-arvot .h-tiedostoon, josta niitä sai käytettyä nRF 5340 -alustalle luodussa neuroverkkoa matkivassa ohjelmassa mittaustulosten luokitteluun.

#### Kuva: Neuroverkkomalli
![Neuroverkkomalli](readme_images/cnn_model.png?raw=true)

### Raspberry Pi
Etsimme bleak-kirjastoon pohjautuvalla ohjelmalla kiihtyvyysanturipalvelumme ympäröivien BLE-mainosten joukosta. Ilmoituksia vastaanottaessaan ohjelmamme purkaa datan ja lähettää sen eteenpäin tietokantaamme mysql-kirjaston avulla.

### Linux-serveri ja MySQL-tietokanta
Saimme koululta allokoidut henkilökohtaiset Linux-serverit, joita käytimme Linuxin opetteluun (ja sitä tukevien lisätehtävien tekoon). Loimme servereillemme myös omat MySQL-tietokannat datan säilytykseen sekä Apache-serveriin perustuvia HTTP-rajapintoja datan hakemiseen tietokannasta.

### Muut taidot
Kurssilla opetettiin scrum-metodologiaa, ja pidimme säännöllisiä ryhmä- ja tiimipalavereja. Tehtävien seuraamisessa apuna oli GitHubin projektiosion kanban-taulu.