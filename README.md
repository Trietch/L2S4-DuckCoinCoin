# L2S4-DuckCoinCoin
## Second year computer student at Paul Sabatier University :fr:
#### Student : Tristan Marrec :shipit: | Tutor : Stergos Afantenos

✔️  Blockchain generation
```
./duckcoincoin DIFFICULTY BLOCKS_NB LOG
```

✔️  Blockchain generation with a cheating block
```
./duckcoincoin DIFFICULTY BLOCKS_NB LOG BLOCK_TO_CHEAT_NB
```

✔️  Blockchain generation with a cheating transaction
```
./duckcoincoin DIFFICULTY BLOCKS_NB LOG BLOCK_TO_CHEAT_TRANSACTION_NB TRANSACTION_TO_CHEAT_NB
```
🚧 (unfinished) Blockchain generation from a JSON file
```
./duckcoincoin JSON_FILE
```
---

Input | Type | Info 
--- | --- | ---
DIFFICULTY | **unsigned long** | *Blockchain difficulty*
BLOCKS_NB | **unsigned long** | *Number of blocks in the blockchain*
LOG | **0 ou 1** | *Print log* 
BLOCK_TO_CHEAT | **unsigned long** | *Number of the cheating block*
BLOCK_TO_CHEAT_TRANSACTION_NB | **unsigned long** | *Number of the cheating transaction block*
TRANSACTION_TO_CHEAT_NB | **unsigned long** | *Number of the cheating transaction in the block*
JSON_FILE | **String** | *JSON file path*

---
👍 Thanks to
- Brad Conte [brad@bradconte.com](brad@bradconte.com)
- udp [@github](https://github.com/udp/json-parser)

respectively for
- sha256.h sha256.c sha256_utils.h sha256_utils.c
- json.c json.h test_json.c libjsonparser.a

---

*[https://github.com/Trietch/L2S4-DuckCoinCoin](https://github.com/Trietch/L2S4-DuckCoinCoin)*
