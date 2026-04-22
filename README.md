# PmergeMe

A fresh implementation of the Ford-Johnson / Merge-Insertion sort for CPP Module 09.

This project is a restart of my earlier attempt. My previous version sorted data, but I misunderstood part of the algorithm, especially the role of the Jacobsthal sequence and the insertion order of pending elements. This rewrite focuses first on correctness and clarity before optimisation.

## Goal

Implement a correct version of Ford-Johnson sort using:

- recursive pairing
- alpha / beta separation
- main-chain construction
- bounded insertion of betas
- Jacobsthal-guided insertion order
- support for both `std::vector` and `std::deque`

## What I misunderstood before

My main confusion was about **where the Jacobsthal sequence should be applied**.

I originally mixed up these ideas:

- the order of elements in the current sorted `mainChain`
- the original pairing order of the pending betas
- the partner relation between each beta and its alpha

### Correct idea

At each recursion level:

1. Values are grouped into pairs.
2. In each pair:
   - `alpha` = larger value
   - `beta` = smaller value
3. The alphas are recursively sorted to form the `mainChain`.
4. The alphas key a record of its betas.
5. The **Jacobsthal sequence is used to choose the order of beta insertions from the mainChain list**.
6. Each beta is inserted by binary search only in the range **before its partner alpha** in the main chain.

So the Jacobsthal order is applied to the ordered **mainChain**

## Why

Alphas move during recursion, at each level, the number of returning mainChain will be made up of the alphas that was passed earlier. 


## Output

The program:

- prints the input before sorting
- prints the sorted result
- measures time for `std::vector`
- measures time for `std::deque`
