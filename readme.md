# Open Function Hook Example

This project demonstrates how to create a hook for the `open` system call using the `LD_PRELOAD` mechanism in Linux. The hook intercepts calls to the `open` function and blocks access to any files in the `/tmp` directory while allowing normal access to all other files.

## How it Works

### LD_PRELOAD Mechanism

The `LD_PRELOAD` environment variable allows you to load a shared library before any other libraries, including the C standard library. This enables you to override standard library functions with your own implementations.

### Project Components

1. **hook.c**
   - Implements the hook for the `open` function
   - Uses `dlsym` to get the address of the original `open` function
   - Checks if the file path starts with "/tmp"
   - Blocks access to files in /tmp by returning -1
   - Forwards all other calls to the original open function
   - Properly handles variable arguments for O_CREAT flag

2. **test.c**
   - A test program that demonstrates the hook functionality
   - Attempts to open a regular file (should succeed)
   - Attempts to open a file in /tmp (should fail when hook is active)

3. **Makefile**
   - Builds the shared library (libhook.so)
   - Builds the test program
   - Provides targets for building and testing

## Building the Project

```bash
make
```

This will create:
- `libhook.so`: The shared library containing the hook
- `test`: The test executable

## Running the Tests

You can run the tests in two ways:

1. Using the make target:
```bash
make run
```

2. Manually:
```bash
# Create a test file
touch test.txt

# Run the test program with the hook
LD_PRELOAD=./libhook.so ./test

# Run without the hook (for comparison)
./test
```

## Expected Output

When running with the hook:
```
Test 1: Successfully opened test.txt
[HOOK] Blocked access to: /tmp/test.txt
Test 2: Access to /tmp/test.txt blocked (expected)
```

Without the hook:
```
Test 1: Successfully opened test.txt
Test 2: Cannot open /tmp/test.txt
```

## Technical Details

### Hook Implementation

The hook works by:
1. Using `dlsym(RTLD_NEXT, "open")` to get the next occurrence of the `open` function in the library search order
2. Checking if the file path starts with "/tmp"
3. Either blocking the access or calling the original function

### Variable Arguments Handling

The hook properly handles the variable argument case when O_CREAT flag is used:
```c
if (flags & O_CREAT) {
    va_list args;
    va_start(args, flags);
    mode_t mode = va_arg(args, mode_t);
    va_end(args);
    return orig_open(pathname, flags, mode);
}
```

## Common Issues and Troubleshooting

1. If the hook isn't working:
   - Make sure libhook.so is in the current directory
   - Verify that LD_PRELOAD is set correctly
   - Check if your system allows LD_PRELOAD (some systems restrict it for security)

2. If compilation fails:
   - Ensure you have gcc installed
   - Make sure you have development libraries installed (particularly libdl)

## System Requirements

- Linux operating system
- GCC compiler
- Development libraries (particularly libdl)

## Cleaning Up

To remove all built files:
```bash
make clean
```

## Security Considerations

This is a demonstration project. In a production environment:
- Consider additional security checks
- Add proper error handling
- Add logging mechanisms
- Consider race conditions
- Handle edge cases (symlinks, relative paths, etc.)
