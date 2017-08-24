# (DDTI) Distributed Decision Tree Induction
Train a model with a decision tree, then measures its predictive accuracy.
Ddti does this in a distributed manner, using MPI. The master node will induct
the decision tree, using slave nodes to execute tasks such as computing
entropies. The algorithm building the decision tree is based on Quilan's
C4.5. The algorithm only handles categorical attributes.

Written in C++14.

## Table of Contents
1. [Usage](#usage)
2. [Deployment](#deployment)
3. [Dependencies](#dependencies)
4. [Design](#design)
5. [Future improvements](#future-improvements)

## Usage
Being an MPI software, Ddti should be executed using mpirun/mpiexec. Here is an
example:

```mpirun -n 4 ./ddti --training_set ../datasets/careval.csv --attributes buying maint doors persons lug_boot safety class -o careval_model.txt```

```
Required input options:

--training_set (-i) [string]
Path to the training dataset. The accepted formats are
CSV (.csv or .txt), ASCII (.txt), Armadillo ASCII (.txt), PGM (.pgm),
PPM (.ppm), Raw binary (.bin), Armadillo binary (.bin).

Optional input options:

--attributes (-a)
List of attribute names, separated by spaces (e.g. -a name lastname age).

--debug (-d)
Prints some debug information such as Information Gain Ratio values.

--help (-h)
Default help info.

--info [string]
Get help on a specific module or option.

--labels_column (-l) [int]
Index of the column containing the labels to predict (must be between 0 and
N-1). If unspecified, the algorithm will use the last column of the dataset.

--min_leaf_size (-m) [int]
Minimum number of instances per leaf. Default value 2.

--test_set (-t) [string]
The dataset used to test the predictive accuracy of the generated model.
If none is provided, the training set will be used. The accepted formats
are CSV (.csv or .txt), ASCII (.txt), Armadillo ASCII (.txt), PGM (.pgm),
PPM (.ppm), Raw binary (.bin), Armadillo binary (.bin).

--verbose (-v)
Display informational messages and the full list of parameters and timers at
the end of execution.

--version (-V)
Display the version of mlpack.

Optional output options:

--model_file (-o) [string]
The path of the file where the model will be dumped. The only supported
output format at the moment is .txt. If unset, the model will be printed on
the standard output.
```

## Deployment
Make sure to install all the dependencies, then execute the following command:

```git clone https://github.com/alexandretea/ddti.git && mkdir ddti/build && cd ddti/build && cmake .. && make```

## Dependencies
- C++14
- An MPI implementation
- Armadillo (for matrices)
- boost_program_options
- MLpack (for Load(), CLI, and Logger)
- CMake

## Design
The processor with the rank 0 hosts the MasterNode. All the other processors
host a SlaveNode. The MasterNode distributes computations -to the slaves-
to build the decision tree.

List of the currently distributed tasks:
- Computation of contingency tables (instance-based distribution)
	* Instances are evenly scattered to every slaves
	* A stream of contingency tables (one per attribute) is reduced using a sum function
- Computation of conditional entropies and split entropies of an attribute
	* The counts of each attribute values are scattered (so each row of a CT)
	* The conditional entropies and split entropies are reduced using a sum function

NOTE: The MasterNode also receives and executes tasks (collective operations).

## Future improvements
- Bufferise the loading and scattering of matrices
- The number of contingency table computations can be reduced (specify list of attributes)
- The entropy computation of the label dimension should be distributed
- Shadow master nodes to improve reliability?
