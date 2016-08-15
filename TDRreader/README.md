# TDRreader
Software to sort binary files in TDR format. See the [documentation](https://vetlewi.github.io/Master/) or the code itself for usage.

### Build

To build the software you can use either qmake or the Makefile included. If you use the Makefile you will need to edit the file such that it has the correct compiler. The software requires [ROOT](https://root.cern.ch/) 5 or 6.


### Input files

The software requires a input file to run. An example of such a runfile is seen below:

<pre>

# This is a comment

# Syntax of detectors list:
# ADC-address name theta-angle phi-angle type telescope
# Example entry:
# 67  Clover1.Ed 90.0 0.0 gamma C1D
# See Detectors class for more information.
detectors /path/to/list/of/detectors.list

# Syntax of gain/shift file:
# ADC-address s g a2 a3 a4
# s: Shift
# g: Gain
# a2: If the gain/shift is not linear, usually it is so this is usually 0
# a3: If the gain/shift is not linear, usually it is so this is usually 0
# a4: If the gain/shift is not linear, usually it is so this is usually 0
# Example of entry:
# 65 1.60683 0.865863 0 0 0
# The energy found in a detector is then:
# E = s + g*adcdata + a2*adcdata^2 + a3*adcdata^3 + a4*adcdata^4
# Example of how to set the gain file in the main input file:
gain file /path/to/list/of/gain_shift.list

# User set particle gates.
# Syntax:
# telescope gate A upper N a0 a1 ... aN lower M b0 b1 ... bN
# Where a0 + a1*(E-energy) + ... + aN*(E-energy)^N
# is the polynomials that are used in the gate. The letter A
# is to indicate that this is the A telescope.
# Example
telescope gate A upper 3 5975.36 -0.59 2.3597e-5 lower 3 5562.92 -0.647 2.8792e-5
telescope gate B upper 3 8606.22 -0.748 2.89e-5 lower 3 7705.22 -0.748 2.89e-5

# User set beta-factor in Doppler corrections.
# Syntax:
# telescope beta A beta-value
# Example:
telescope beta A 0.0835
telescope beta B 0.0835

# Mapping of ADC-address to strip number.
# Syntax of mapping file:
# strip-no adc-address
# Example entry:
# 8 328
# Input line in the main input file (this file)
telescope strips A /Path/To/Vertical/Strips/MapA.txt /Path/To/Horizontal/Strips/MapA.txt
telescope strips A /Path/To/Vertical/Strips/MapB.txt /Path/To/Horizontal/Strips/MapB.txt

# Set maximum limit for particle energy (discard punch-through)
# Syntax:
# MaxEA max-Energy-in-keV
# MaxEB for telescope B.
MaxEA 13000.0
MaxEB 14000.0

# Set files containing the time gates.
# Two gates needs to be set for each telescope.
# The syntax of the entries in the gate files:
# adc-address minT maxT
# Where minT is in units of the timestamp.
# The files is set as:
TimeGateMain A /Path/To/First/Time/Gate/FileA.list
TimeGateMain B /Path/To/First/Time/Gate/FileB.list
TimeGateSec A /Path/To/Second/Time/Gate/FileA.list
TimeGateSec B /Path/To/Second/Time/Gate/FileB.list
TimeGateBG A /Path/To/Background/Gate/For/Background/SubtractionA.list
TimeGateBG B /Path/To/Background/Gate/For/Background/SubtractionB.list


# Set file with polynomials used to calculate the excitation energy.
# File entry syntax:
# vertical-strip_No horizontal-strip_No a0 a1 a2
# Example entry:
# 2 8 8.49535 -0.593602 -0.00767706 
# Syntax of main input file entry:
telescope excitation A /Path/To/File/With/PolynomialsA.txt
telescope excitation B /Path/To/File/With/PolynomialsB.txt

# Set file containing the raw data to sort.
data file /Path/To/File

# Output of histograms:
# Export to MAMA files:
export mama hist_name /Path/To/Output/File
# Export to ROOT file (will dump all histograms, allways run this AFTER all MAMA export lines):
export root /Path/To/Root/File.root

</pre>