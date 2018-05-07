# L2S4-DuckCoinCoin
## L2 Informatique Université Paul Sabatier
#### Auteur/Etudiant : Tristan Marrec :shipit: | Tuteur : Stergos Afantenos

✔️  Génération d'une blockchain
```
./duckcoincoin DIFFICULTY BLOCKS_NB LOG
```

✔️  Génération d'une blockchain avec cheating d'un block
```
./duckcoincoin DIFFICULTY BLOCKS_NB LOG BLOCK_TO_CHEAT_NB
```

✔️  Génération d'une blockchain avec cheating d'une transaction
```
./duckcoincoin DIFFICULTY BLOCKS_NB LOG BLOCK_TO_CHEAT_TRANSACTION_NB TRANSACTION_TO_CHEAT_NB
```
🚧 (non fini) Génération d'une blockchain a partir d'un fichier JSON
```
./duckcoincoin JSON_FILE
```
---

Entrée | Type | Définition 
--- | --- | ---
DIFFICULTY | **unsigned long** | *Difficulté de la blockchain*
BLOCKS_NB | **unsigned long** | *Nombre de blocks de la blockchain*
LOG | **0 ou 1** | *Afficher ou non des informations* 
BLOCK_TO_CHEAT | **unsigned long** | *Numéro du block à cheater*
BLOCK_TO_CHEAT_TRANSACTION_NB | **unsigned long** | *Numéro du block à cheater une transaction*
TRANSACTION_TO_CHEAT_NB | **unsigned long** | *Numéro de la transaction à cheater*
JSON_FILE | **String** | *Chemin du fichier JSON*

---
👍 Merci à 
- Brad Conte [brad@bradconte.com](brad@bradconte.com)
- udp [@github](https://github.com/udp/json-parser)

pour respectivement
- sha256.h sha256.c sha256_utils.h sha256_utils.c
- json.c json.h test_json.c libjsonparser.a

---

*[https://github.com/Trietch/L2S4-DuckCoinCoin](https://github.com/Trietch/L2S4-DuckCoinCoin)*
