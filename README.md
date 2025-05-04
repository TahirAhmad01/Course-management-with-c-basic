# Course Management System

A C-based console application for managing student, instructor, and course records in an educational institution. The system allows users to add, delete, modify, and display records, with data stored persistently in text files.

## Features
- **Student Management**:
  - Add new students with roll number, name, branch, semester, and enrolled courses.
  - Delete or modify existing student records.
  - Display all students or a specific student by roll number.
- **Instructor Management**:
  - Add new instructors with ID, name, and taught courses.
  - Delete or modify instructor records.
  - Display all instructors or a specific instructor by ID.
- **Course Management**:
  - Add new courses with code, name, credits, instructors, and enrolled students.
  - Delete or modify course records.
  - Display all courses or a specific course by code.
- **Persistent Storage**:
  - Records are stored in text files (`students.txt`, `instructors.txt`, `courses.txt`).
  - Data is loaded at startup and saved after modifications.
- **Error Handling**:
  - Validates input for branch codes, unique IDs, and file operations.
  - Handles memory allocation failures and file access errors.

## Prerequisites
- **C Compiler**: GCC or any C99-compliant compiler.
- **Operating System**: Tested on Linux/macOS (should work on Windows with minor path adjustments).
- **Optional**: `valgrind` for memory leak checking.

## Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/course-management-system.git
   cd course-management-system
   ```

2. **Compile the Code**:
   ```bash
   gcc -Wall -o course_management course_management.c
   ```

3. **Prepare Input Files**:
   Create the following text files in the project directory with the specified format:
   - `students.txt`:
     ```
     2
     001
     John Doe
     0
     4
     2
     CS101 CS102
     002
     Jane Smith
     1
     3
     1
     EE201
     ```
   - `instructors.txt`:
     ```
     1
     I001
     Dr. Smith
     2
     CS101 CS102
     ```
   - `courses.txt`:
     ```
     2
     CS101
     Introduction to Programming
     3
     1
     I001
     2
     001 002
     CS102
     Data Structures
     3
     1
     I001
     1
     001
     ```

## Usage
1. **Run the Program**:
   ```bash
   ./course_management
   ```

2. **Menu Options**:
   - Select a category:
     - `1`: Students
     - `2`: Instructors
     - `3`: Courses
   - Choose an action:
     - `1`: Display all records
     - `2`: Display a specific record
     - `3`: Add a new record
     - `4`: Delete a record
     - `5`: Modify a record
   - Follow the prompts to enter data (e.g., roll number, course code).

3. **Example**:
   To add a new student:
   - Select `1` (Students), then `3` (Add).
   - Enter roll number, name, branch code (0=CSE, 1=EE, 2=ME), semester, and course codes.
   - If a course doesn't exist, you'll be prompted to add it.

4. **Check Output**:
   - Records are updated in the respective text files after each operation.
   - Use `cat students.txt` to view changes.

## File Format
- **students.txt**:
  - First line: Number of students.
  - For each student:
    - Roll number
    - Name
    - Branch code (0=CSE, 1=EE, 2=ME)
    - Semester
    - Number of courses
    - List of course codes
- **instructors.txt**:
  - First line: Number of instructors.
  - For each instructor:
    - ID
    - Name
    - Number of courses
    - List of course codes
- **courses.txt**:
  - First line: Number of courses.
  - For each course:
    - Code
    - Name
    - Credits
    - Number of instructors
    - List of instructor IDs
    - Number of students
    - List of student roll numbers

## Testing
1. **Compile with Warnings**:
   ```bash
   gcc -Wall -o course_management course_management.c
   ```
   Ensure no warnings are reported.

2. **Run Tests**:
   - Test all menu options (add, delete, modify, display).
   - Verify file updates after each operation.
   - Check for crashes or invalid inputs.

3. **Memory Leak Check**:
   ```bash
   valgrind ./course_management
   ```
   Confirm no memory leaks are reported.

## Troubleshooting
- **File Not Found**: Ensure `students.txt`, `instructors.txt`, and `courses.txt` exist in the project directory.
- **Segmentation Fault**: Verify input file format and check for invalid inputs (e.g., non-numeric branch codes).
- **Memory Issues**: Run with `valgrind` to diagnose leaks or invalid memory access.

## Contributing
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make changes and commit (`git commit -am 'Add feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Create a Pull Request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.