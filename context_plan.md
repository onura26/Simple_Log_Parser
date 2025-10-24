[line1, line2, line3, line4, line5]  <- size 5
                          ↑
                     current read position
```
When you find a match, dump the buffer. When buffer is full and no match, oldest line gets yeeted.

**For `-A` (after context):**
When you find a match, set a **counter** = N. Print the next N lines regardless of whether they match. Decrement counter each line.

**For `-C` (combined context):**
Just `-B N` + `-A N` at the same time.

## The Tricky Part: Overlapping Contexts

What if you have:
```
line1
line2  <- match
line3
line4  <- match (but line3 is still in "after" mode from previous match)
line5
```

You need to track:
- "Am I still printing after-context from a previous match?"
- "Should I avoid printing duplicates?"

**Solution:** Keep a `lastPrintedLine` number. If current line was already printed, skip it.

## Data Structures You Need

1. **`std::deque<std::string>`** - for the before-context buffer (efficient push_back/pop_front)
2. **`int afterContextRemaining`** - counter for lines left to print
3. **`int lastPrintedLineNum`** - avoid duplicate prints
4. **`bool inAfterContext`** - flag to know if you're in "just printing context" mode

## The Flow
```
for each line:
    add line to deque (size = B)
    
    if line matches pattern:
        print all lines in deque (the "before" context)
        print current line (with color)
        set afterContextRemaining = A
        clear deque
    
    else if afterContextRemaining > 0:
        print line (no color, it's just context)
        decrement afterContextRemaining
    
    if deque is full:
        pop oldest line from deque