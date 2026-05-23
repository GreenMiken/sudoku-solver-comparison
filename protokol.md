# BI-ZUM — Semestrální práce
## Sudokář – klikař

V rámci semestrální práce se zabývám porovnáním pěti různých algoritmů pro řešení Sudoku. Tři algoritmy jsou různé implementace algoritmu **stoupání do kopce (Hill Climbing)**. Jeden algoritmus využívá **simulované žíhání (Simulated Annealing)** a poslední algoritmus je zástupce **systematického prohledávání**, konkrétně **backtracking**, který slouží k porovnání s ostatními lokálně prohledávajícími algoritmy.

---

# Implementované algoritmy

## 1. SolverHillClimbRandom

Algoritmus stoupání do kopce, který vždy vybere dvě náhodné buňky v jednom bloku a ty následně vymění.

Účelová funkce je počet konfliktů v celé mřížce.

---

## 2. SolverHillClimbConflictCells

Také algoritmus stoupání do kopce, ale na rozdíl od předchozího vybírá vždy náhodnou konfliktní buňku a k ní druhou konfliktní buňku (pokud existuje) ve stejném bloku.

Účelová funkce je stejná — počet konfliktů v celé mřížce.

---

## 3. SolverHillClimbWorstBlock

Algoritmus vybírá blok s největším počtem konfliktních buněk a v něm následně vymění dvě náhodné buňky.

---

## 4. SolverSimulatedAnnealing

Algoritmus simulovaného žíhání. Vybírá vždy dvě náhodné buňky, které vymění (stejně jako `SolverHillClimbRandom`), ale občas přijme i horší stav.

Účelová funkce je opět počet konfliktů v mřížce.

Algoritmus začíná s určitou teplotou, která se postupně snižuje. Pokud dlouhou dobu nedojde ke zlepšení stavu, dojde k tzv. **reheatingu**, tedy navrácení teploty na původní hodnotu.

---

## 5. SolverBacktracking

Klasický algoritmus systematického prohledávání.

Postupně zkouší všechny možné hodnoty a při nalezení konfliktu se vrací zpět. Na jednodušších instancích Sudoku dosahuje velmi dobrých výsledků, avšak na větších a řidších mřížkách začíná výrazně zpomalovat.

---

# Testy

V rámci semestrální práce bylo implementováno pět druhů testů:

---

## 1. `Benchmark::test`

Metoda spustí každý algoritmus pro danou mřížku několikrát (`iterations`) a zaznamená výsledky. Následně vypočítá:

- průměrnou dobu výpočtu,
- úspěšnost algoritmu (`success rate`).

Úspěšnost je důležitá, protože algoritmus nemusí Sudoku stihnout vyřešit v rozumném čase.

---

## 2. `Benchmark::testWithoutBT`

Stejný test jako předchozí, ale bez algoritmu backtracking.

Slouží především k testování mapy `grid4.txt`, která obsahuje velké množství prázdných políček a pro backtracking nemusí být vhodná.

---

## 3. `Benchmark::simAnnealTest`

Test slouží k nalezení optimální teploty a `cooling rate` pro algoritmus simulovaného žíhání.

Metoda pro danou Sudoku mapu vyzkouší všechny kombinace definovaných teplot a hodnot `cooling rate` a následně vypíše výsledky.

---

## 4. `Benchmark::LSRestartTest`

Test slouží k nalezení optimální hodnoty restartu pro algoritmy lokálního prohledávání.

Zkoumá tedy, po kolika neúspěšných iteracích by mělo dojít k restartu algoritmu.

---

## 5. `Benchmark::SAvsBT`

Cílem je porovnat simulované žíhání oproti backtrackingu, zejména na mřížce grid4,
která je pro backtracking už celkem komplikovaná, protože je řídká.

---

# Mřížky

Ve složce `grids` se nachází čtyři různé Sudoku mapy.

Na prvních třech mapách (`grid1` až `grid3`) dominuje algoritmus backtracking. Na mapě `grid4.txt` však backtracking zaostává oproti simulovanému žíhání, protože obsahuje velké množství prázdných políček.

---

# Výsledky

Překvapilo mě, že `SolverHillClimbRandom` poměrně jednoznačně porážel ostatní dvě „chytřejší“ heuristiky, tedy `SolverHillClimbConflictCells` a `SolverHillClimbWorstBlock`.


`SolverHillClimbConflictCells` si vedl na jednodušších mřížkách ještě relativně dobře, ale na složitějších už byl výrazně pomalejší.

`SolverHillClimbConflictCells` si vedl dobře na grid2, ale jinak byl pomalejší.

`SolverHillClimbWorstBlock` byl vždy nejpomalejší ze všech algoritmů. Tedy tato heuristika není dobrá.

Z algoritmů lokálního prohledávání vyhrálo simulované žíhání.

Na jednodušších mřížkách (`grid1` až `grid3`) jednoznačně dominoval backtracking. Avšak na řídké mřížce jako je grid4
jednoznačně vyhrává simulované žíhání. Pokud by byla mřížka ještě více řídká, pak by byl rozdíl mezi simulovaným žíháním a backtrackingem ještě větší.

výsledky jednotlivých testů jsou ve složce img

---

# Nejlepší nalezené parametry

## Simulated Annealing

SA preferovalo menší teplotu a rychlejší ochlazování. Nejlepší nalezené parametry byly:

- **teplota** vycházela nejlépe jako hodnota `1.0` s **cooling rate** `0.99`

---

## Restart lokálního prohledávání

Nejlepší hodnota restartu (tedy po kolika neúspěšných iteracích se má algoritmus restartovat) vycházela přibližně mezi:
hodnotami `500` až `1000`. Akorát WorstBlock heuristika preferovala častější restarty okol hodnoty `100`.

---

# Implementace

Každý solver dědí z AbstractSudokuSolver, která má abstraktní metodu solve. Z této abstraktní třídy dědí AbstractLocalSearchSolver, ze kterého dědí
všechny solvery lokálního prohledávání.

Třída Benchmark provádí všechny testy a samotné solvery si uchovává v hash mapě. Pro vytváření solverů používá factory funkci createSolver.
Třídá využívá polymorfismu a se solvery pracuje jako s pointery na AbstractSudokuSolver, či na AbstractLocalSearchSolver
