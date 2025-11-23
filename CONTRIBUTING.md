# Contributing Guidelines

## Environment Setup

### Using Conda

1. **Create conda environment**
   ```bash
   conda create -n connect-four
   conda activate connect-four
   
   # Install dependencies
   conda install cmake make compilers
   ```

## Development Workflow

1. **Activate conda environment**
   ```bash
   conda activate connect-four
   ```

2. **Pull latest main before starting**
   ```bash
   git checkout main
   git pull origin main
   ```

3. **Create feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

4. **Work on your module**
   - Person 1: `board.c/h` + tests
   - Person 2: `game.c/h` + tests
   - Person 3: `ai.c/h` + tests
   - Person 4: `history.c/h` + tests
   - Together: `main.c`, `io.c/h`, `utils.c/h`

5. **Build and test locally**
   ```bash
   # Clean previous build (optional)
   rm -rf build
   
   # Configure with CMake (out-of-source build)
   cmake -B build
   
   # Build
   cmake --build build
   
   # Run tests
   cd build && ctest --output-on-failure
   
   # Or run tests directly
   ./build/tests/board_tests
   
   # Run the game
   ./build/connect4
   ```

6. **Commit with clear messages**
   ```bash
   git add .
   git commit -m "feat: implement board_check_winner function"
   ```

7. **Push and create Pull Request**
   ```bash
   git push origin feature/your-feature-name
   ```
   - Create PR on GitHub targeting `main` branch
   - Request review from team members
   - Wait for approval before merging

## Module Dependencies

- **Board** (Person 1): No dependencies → **Merge First**
- **Game** (Person 2): Depends on board
- **Utils** (Together): Depends on board
- **I/O** (Together): Depends on board, game
- **History** (Person 4): Depends on board
- **AI** (Person 3): Depends on board, game

## Merge Order

```
1. feature/board-implementation → main (Person 1)
2. feature/game-logic → main (Person 2)
3. feature/history-module → main (Person 4)
4. feature/ai-minimax → main (Person 3)
5. feature/io-and-utils → main (Together)
```

## Resolving Merge Conflicts

If `main` has moved ahead while you were working:

```bash
# Update your feature branch
git checkout feature/your-feature-name
git fetch origin
git rebase origin/main

# Resolve any conflicts, then
git add .
git rebase --continue

# Force push (since history changed)
git push --force-with-lease origin feature/your-feature-name
```

## Code Review Checklist

Before creating a Pull Request:

- [ ] Code compiles without warnings
- [ ] All tests pass (`ctest --output-on-failure`)
- [ ] Functions have documentation comments
- [ ] No memory leaks (run with `valgrind` if available)
- [ ] Follows project naming conventions
- [ ] Added tests for new functionality
- [ ] Updated this CONTRIBUTING.md if workflow changes

## CMake Build Commands

```bash
# Configure (from project root)
cmake -B build

# Build
cmake --build build

# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Release build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Clean and rebuild
rm -rf build
cmake -B build
cmake --build build
```

## Running Tests

```bash
# Run all tests via CTest
cd build && ctest --output-on-failure

# Run tests directly (more verbose output)
./build/tests/test_board

# Run with valgrind (check for memory leaks)
valgrind --leak-check=full ./build/tests/test_board
```

## Troubleshooting

### Build fails with "No such file or directory"

Make sure all stub files exist:
```bash
# Create empty stub files if missing
touch src/game.c src/ai.c src/utils.c src/history.c src/io.c
```

### Tests won't compile

```bash
# Clean build and reconfigure
rm -rf build
cmake -B build
cmake --build build
```

### Conda environment issues

```bash
# Deactivate and reactivate
conda deactivate
conda activate connect-four

# Verify environment
which cmake  # Should point to conda's cmake
```

### Can't find include files

Make sure you're building from the project root:
```bash
cd /Users/salomeshioshvili/connect-four
cmake -B build
cmake --build build
```
