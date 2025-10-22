# Contributing to STM32 Modbus Library

Thank you for your interest in contributing to this project! This document provides guidelines and instructions for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Pull Request Process](#pull-request-process)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Enhancements](#suggesting-enhancements)

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When creating a bug report, include:

- **Clear title and description**
- **Steps to reproduce** the issue
- **Expected behavior** vs actual behavior
- **Hardware setup** (MCU model, peripherals)
- **Software versions** (STM32Cube, HAL version)
- **Code snippets** if applicable
- **Serial logs or screenshots**

**Bug Report Template:**
```markdown
**Description:**
Brief description of the bug

**Hardware:**
- MCU: STM32F407VG
- Peripheral: RS-485 with MAX485
- Board: Custom/Discovery/Nucleo

**Software:**
- STM32CubeIDE version: 1.x
- HAL version: 1.x
- Library version: commit hash or tag

**Steps to Reproduce:**
1. Initialize with...
2. Call function...
3. Observe...

**Expected Behavior:**
What should happen

**Actual Behavior:**
What actually happens

**Logs:**
```
Paste relevant logs here
```

**Additional Context:**
Any other relevant information
```

### Suggesting Enhancements

Enhancement suggestions are welcome! Please provide:

- **Clear use case**: Why is this enhancement needed?
- **Detailed description**: What should it do?
- **Examples**: How would it be used?
- **Alternatives considered**: Other approaches you've thought about

### Contributing Code

We welcome code contributions! Areas where help is especially appreciated:

1. **Bug fixes**
2. **New Modbus functions** (ASCII protocol, diagnostics)
3. **Additional examples**
4. **Hardware support** (new MCU series, PHY chips)
5. **Documentation improvements**
6. **Unit tests**
7. **Performance optimizations**

## Development Setup

### Prerequisites

- STM32CubeIDE or command-line ARM GCC toolchain
- Git
- Text editor or IDE
- STM32 development board
- RS-485 or Ethernet hardware

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:
```bash
git clone https://github.com/YOUR_USERNAME/modbus.git
cd modbus
```

3. Add upstream remote:
```bash
git remote add upstream https://github.com/nguyentiendungIoT/modbus.git
```

4. Create a feature branch:
```bash
git checkout -b feature/your-feature-name
```

### Building

For testing changes, integrate into an STM32 project:

1. Create new STM32 project in STM32CubeIDE
2. Copy or link source files
3. Build and test on hardware

## Coding Standards

### C Code Style

Follow these conventions for consistency:

**File Headers:**
```c
/**
 * @file filename.c
 * @brief Brief description
 * @author Your Name
 * @date Year
 */
```

**Function Documentation:**
```c
/**
 * @brief Brief description of function
 * @param param1 Description of parameter
 * @param param2 Description of parameter
 * @return Description of return value
 */
modbus_error_t function_name(type param1, type param2)
{
    // Implementation
}
```

**Naming Conventions:**
- Functions: `modbus_rtu_function_name()` (snake_case with prefix)
- Variables: `variable_name` (snake_case)
- Constants: `MODBUS_CONSTANT_NAME` (UPPER_SNAKE_CASE)
- Types: `modbus_type_t` (snake_case with _t suffix)
- Structs: `modbus_struct_name_t`

**Code Style:**
- Indentation: 4 spaces (no tabs)
- Braces: Opening brace on same line (K&R style)
- Line length: Prefer < 100 characters
- Comments: Use `/* */` for multi-line, `//` for single line

**Example:**
```c
modbus_error_t modbus_rtu_read_registers(modbus_rtu_t *mb, uint8_t slave_id,
                                         uint16_t start_addr, uint16_t count)
{
    if (!mb || count == 0) {
        return MODBUS_ERROR_INVALID_PARAM;
    }
    
    /* Build request frame */
    mb->tx_buffer[0] = slave_id;
    mb->tx_buffer[1] = MODBUS_FC_READ_HOLDING_REGISTERS;
    
    for (uint16_t i = 0; i < count; i++) {
        // Process each register
    }
    
    return MODBUS_OK;
}
```

### Header Guards

Use include guards in all header files:
```c
#ifndef MODBUS_FILENAME_H
#define MODBUS_FILENAME_H

// Content

#endif /* MODBUS_FILENAME_H */
```

### Error Handling

- Always check function parameters
- Return appropriate error codes
- Don't use `assert()` in production code
- Log errors when debug mode is enabled

### Memory Management

- Avoid dynamic memory allocation (malloc/free)
- Use static allocation or caller-provided buffers
- Document buffer size requirements
- Check buffer bounds

## Pull Request Process

### Before Submitting

1. **Test thoroughly** on real hardware
2. **Update documentation** if adding features
3. **Follow coding standards**
4. **Add examples** if appropriate
5. **Update README** if needed
6. **Commit messages** should be clear and descriptive

### Commit Message Format

Use clear, descriptive commit messages:

```
Short summary (50 chars or less)

More detailed explanation if needed. Wrap at 72 characters.
Explain what changed and why, not how (code shows how).

- Bullet points are okay
- Use imperative mood: "Add feature" not "Added feature"

Fixes #123
```

**Examples:**
- `Add support for Modbus ASCII protocol`
- `Fix CRC calculation for frames > 128 bytes`
- `Update API documentation for TCP functions`
- `Add example for multi-slave RTU master`

### Creating Pull Request

1. **Push to your fork:**
```bash
git push origin feature/your-feature-name
```

2. **Create pull request** on GitHub

3. **Fill in PR template:**
   - Description of changes
   - Related issue numbers
   - Testing performed
   - Hardware tested on
   - Breaking changes (if any)

4. **Wait for review** and address feedback

### Code Review

All submissions require review. Reviewers will check:

- Code quality and style
- Functionality and correctness
- Test coverage
- Documentation completeness
- Performance implications
- Breaking changes

### Merging

Once approved:
- PR will be merged by maintainers
- Contributor will be credited
- Changes will appear in next release

## Testing

### Manual Testing

Test on real hardware:
- Various STM32 families (F1, F4, F7, H7)
- Different baud rates and configurations
- Edge cases (timeouts, errors, max values)
- Master and slave modes
- Multiple concurrent operations

### Test Checklist

- [ ] Compiles without warnings
- [ ] Works on target hardware
- [ ] Tested with real Modbus devices
- [ ] Error cases handled properly
- [ ] No memory leaks
- [ ] Documentation updated
- [ ] Examples still work

## Documentation

When contributing, update relevant documentation:

- **API.md** - For new functions or changed APIs
- **SETUP.md** - For installation or setup changes
- **HARDWARE.md** - For new hardware support
- **README.md** - For major features or changes
- **Examples** - Add or update examples

Use clear, concise language. Include code examples where helpful.

## Community

### Getting Help

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For questions and ideas
- **Email**: For private concerns

### Recognition

Contributors will be:
- Listed in CONTRIBUTORS file
- Mentioned in release notes
- Credited in commit history

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

If you have questions about contributing, please:
1. Check existing documentation
2. Search closed issues
3. Ask in GitHub Discussions
4. Contact maintainers

Thank you for contributing to make this library better!
