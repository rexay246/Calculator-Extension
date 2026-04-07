# Homework: Extend the Calculator with Unit Conversion (Miles ⇄ Feet ⇄ Centimeters)

## 📖 Overview

In this assignment, you will extend the calculator pipeline.
We’ve provided:

* A **C++ DLL project** (`ArithmeticAPI`) with `#TODO:` markers.
* A **C# WinForms frontend** (`CalculatorGUI`) with `#TODO:` markers.

Your job is to **finish the missing pieces** on both sides:

1. Complete the unit conversion logic in C++.
2. Wire up the frontend (WinForms) to call into your DLL.

---

## 🎯 Learning Goals

* Practice exporting functions from a C++ DLL and using them from C#.
* Understand how enums and indices must match between DLL and frontend.
* Implement a simple unit conversion pipeline.
* Get experience with basic WinForms programming.

---

## 🛠 What’s Provided

```
/CalculatorSolution
  /ArithmeticAPI           <-- C++ DLL project
    ArithmeticAPI.h        <-- scaffold with #TODOs
    ArithmeticAPI.cpp      <-- scaffold with #TODOs
  /CalculatorGUI           <-- C# WinForms project
    CalculatorGUI.cs       <-- scaffold with #TODOs
    Program.cs
  README.md
```

---

## 📌 Your Tasks

### A) C++ DLL (ArithmeticAPI)

Open **ArithmeticAPI.h / ArithmeticAPI.cpp** and finish the TODOs.

1. **UnitType enum**

   * Already contains `Miles`, `Feet`, and `Centimeters`.
   * Do not change the values; indices must stay the same.

2. **Conversion factors**

   * Fill in the `UnitFactorToCm` function with correct factors:

     * 1 mile = 160934.4 cm
     * 1 foot = 30.48 cm
     * 1 cm = 1.0 cm

3. **Conversion math**

   * In `ConvertUnits`, finish the calculation:

     * Convert input to centimeters.
     * Convert centimeters to the target unit.
   * Return the final result.

---

### B) C# Frontend (CalculatorGUI)

Open **CalculatorGUI.cs** and finish the TODOs.

1. **P/Invoke for ConvertUnits**

   * Add the `[DllImport]` declaration for the DLL function.
   * Signature: `double ConvertUnits(double value, int fromUnit, int toUnit)`.
   * Ensure the indices match the enum in C++:

     * 0 = Miles, 1 = Feet, 2 = Centimeters.

2. **Populate dropdowns**

   * Add `"Miles"`, `"Feet"`, `"Centimeters"` to both dropdowns (`fromUnit` and `toUnit`).
   * Set sensible defaults: `fromUnit = Miles (0)`, `toUnit = Feet (1)`.

3. **Wire the convert button**

   * Connect the **Convert** button to the handler.
   * TODO hint is provided.

---

## ✅ Testing

* Build the DLL (`Release`, `x64`) → copy `ArithmeticAPI.dll` next to the WinForms `.exe`.
* Run the WinForms app:

  * Test converting 1 mile → feet = **5280**
  * Test converting 1 foot → cm = **30.48**
  * Test converting 1 mile → cm = **160,934.4**
  * Same-unit conversion should return the same number.

---

## 📂 Submission

Upload:

* `ArithmeticAPI/ArithmeticAPI.h`
* `ArithmeticAPI/ArithmeticAPI.cpp`
* `CalculatorGUI/CalculatorGUI.cs`

❌ Do not submit binaries (`.dll`, `.exe`, `.pdb`).

---

## 💡 Tips

* Stick to doubles for math.
* Don’t overcomplicate the C# side — just fill in the TODOs.
* The dropdown order must match the C++ enum exactly.

---
