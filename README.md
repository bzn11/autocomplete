# Autocomplete

A fast autocomplete tool written in **C**. It matches query prefixes against a list of weighted terms using **binary search**, returning the top results sorted by **descending weight**.

* **Case-sensitive** matching
* Matches only the **start** of terms
* Terms list is **loaded and sorted once** for fast repeated queries


---
The input file should contain one term per line in the following format:

```
<weight> <term>
```

#### Example Input

```
10000 Engineering  
9500 English  
8900 EngSci  
9200 Science
```

---

#### Example Output

**Query**: `Eng`

**Matches**:

```
Engineering (10000)  
English (9500)  
EngSci (8900)
```

---
