# Gen IV Battle Hall Analyzer

A multi-threaded, deterministic battle simulation system written in C++ for analyzing Pokémon performance in the Gen IV Battle Hall.

The system evaluates 1v1 battles by modeling opponent behavior using worst-case assumptions and heuristics to estimate survival potential across a streak-based format.

## Features

- Deterministic Gen IV battle simulation engine
- Heuristic-based opponent decision modeling
- Worst-case assumption framework for player evaluation
- Reduced RNG for consistent outcome comparison
- Pokémon performance analysis in Battle Hall format
- Optimal matchup assignment using the Hungarian algorithm for removing less-optimal paths

## Progression

The system began by testing a single Pokémon (Azurill), which surprisingly demonstrated strong streak potential under deterministic assumptions.

This led to broader exploration of:
- Pokémon viability in 1v1 environments
- heuristic evaluation of battle outcomes for other formats
