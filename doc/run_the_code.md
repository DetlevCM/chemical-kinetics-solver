# How to Run the Code

## Requirements

Assuming you have successfully built the code, you require essentially two input files.
The chemical reaction mechanism, in a file called `chem.inp` and the input file called `initial.inp`.

Then it can just be run by calling the executable from the command line in the directory, namely `CKS.exe`.
The code will write a file called `concentrations.txt` as output, as well as additional output file depending on the functionality requested.

### chem.inp

This file needs to follow the chemkin input format and needs to contain the species, thermodynamic data and reactions.

At present, only reversible and irreversible Arrhenius reactions with optional duplicates are supported.

This body reactions are work in progress and not validated/tested.

### initial.inp

The layout for the initial input file follows the ideas of xml style blocks that specify specific parameters.
The chemkin standard exclamation mark, `!`, is used as a comment character, however comments can also be added between the input blocks.

A minimum sample input for a mechanism involving oxygen and dodecane could look at follows:

```text
<Initial Conditions>
Temperature 423.0

!EndTime 1e1 1e0
EndTime 5e4 1e1
</Initial Conditions>
The times given are the end time and output time step.
Multiple entries are possible to adjust the output time step.

<Solver Parameters>
!irrev
!Print Rates
</Solver Parameters>
Select whether the reaction mechanism should be made irreversible before solving.
Option to print reaction rates during the run (creates a large file).

<Mechanism Reduction>
!ReduceReactions 5
</Mechanism Reduction>
Rates based removal of reactions, select the order of magnitude threshold to consider a reaction significant.

<Analysis>
!RatesMaxAnalysis
!StreamRatesAnalysis
!RatesSpeciesAllAnalysis
</Analysis>
Write out additional information about reactions rates.

<Species>
O2(2) 0.002
C12H26(1) 4.7
</Species>
Provide the initial concentrations of species in mol/L units.
```
