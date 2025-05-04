#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

enum branch
{
	CSE,
	EE,
	ME
};

struct student
{
	char rollno[MAX];
	char name[MAX];
	enum branch branch;
	int semester;
	int ncourses;
	char **course_codes;
};

struct instructor
{
	char id[MAX];
	char name[MAX];
	int ncourses;
	char **courses_codes;
};

struct course
{
	char code[MAX];
	char name[MAX];
	int credits;
	int ninstructors;
	char **instructor_ids;
	int nstudents;
	char **student_rollnos;
};

// Global variables
struct student *students = NULL;
struct instructor *instructors = NULL;
struct course *courses = NULL;
FILE *fps, *fpi, *fpc, *buffer;
char students_file[MAX] = "./students.txt";
char courses_file[MAX] = "./courses.txt";
char instructors_file[MAX] = "./instructors.txt";
int ncourses = 0, nstudents = 0, ninstructors = 0;

// Function prototypes
void load_records();
void load_students();
void load_instructors();
void load_courses();
void print_students();
void print_student(struct student s);
void add_student(struct student s);
void addstudent(char *rollno, char *name, int b, int semester, int courses, char **course_codes);
void deletestudent();
void modifystudent();
void printmodifystudent();
void print_instructors();
void print_instructor(struct instructor i);
void add_instructor(struct instructor i);
void addinstructor(char *id, char *name, int ncourses, char **course_codes);
void deleteinstructor();
void modifyinstructor();
void printmodifyinstructor();
void print_courses();
void print_course(struct course c);
void add_course(struct course c);
void addcourse(char *code, char *name, int credits, int ninstructors, char **instructor_ids, int nstudents, char **student_rollnos);
void deletecourse();
void modifycourse();
void printmodifycourse();
void free_all();

int main()
{
	int i, j;
	char rollno[MAX];
	char name[MAX];
	int b;
	int semester;
	char **course_codes;
	char id[MAX];
	char **courses_codes;
	char code[MAX];
	int credits;
	char **instructor_ids;
	char **student_rollnos;
	int m = 0;
	int f = 1;

	load_records();
	printf("Enter the number in which you want to perform changes:\n1.Students\n2.Instructors\n3.Courses\n>>>>>>");
	scanf("%d", &i);
	printf("Enter number for following steps:\n1.Display All\n2.Display\n3.Add\n4.Delete\n5.Modify\n>>>>>");
	scanf("%d", &j);

	if (i == 1)
	{
		switch (j)
		{
		case 1:
			print_students();
			break;
		case 2:
			printf("Enter Roll No.:");
			scanf("%s", rollno);
			for (int i = 0; i < nstudents; i++)
			{
				if (strcmp(rollno, students[i].rollno) == 0)
				{
					print_student(students[i]);
					m = 1;
					break;
				}
			}
			if (m == 0)
			{
				printf("\nEnter valid Roll no.\n");
			}
			break;
		case 3:
			printf("Enter Roll No.:\n");
			scanf("%s", rollno);
			for (int i = 0; i < nstudents; i++)
			{
				if (strcmp(rollno, students[i].rollno) == 0)
				{
					printf("Student already exists.\n");
					f = 0;
					break;
				}
			}
			if (f == 1)
			{
				printf("Enter name:\n");
				scanf(" %[^\n]s", name);
				printf("Enter Branch Code (0=CSE, 1=EE, 2=ME):\n");
				scanf("%d", &b);
				if (b < 0 || b > 2)
				{
					printf("Invalid branch code.\n");
					break;
				}
				printf("Enter Semester Number:\n");
				scanf("%d", &semester);
				int nco;
				printf("Enter no.of courses enrolled:\n");
				scanf("%d", &nco);
				course_codes = malloc(nco * sizeof(char *));
				if (!course_codes)
				{
					perror("Memory allocation failed");
					exit(1);
				}
				printf("Enter Course codes:\n");
				for (int p = 0; p < nco; p++)
				{
					course_codes[p] = malloc(MAX * sizeof(char));
					if (!course_codes[p])
					{
						perror("Memory allocation failed");
						exit(1);
					}
					scanf("%s", course_codes[p]);
				}
				addstudent(rollno, name, b, semester, nco, course_codes);

				// Check if all course codes exist
				int valid_courses = 0;
				for (int j = 0; j < nco; j++)
				{
					for (int l = 0; l < ncourses; l++)
					{
						if (strcmp(courses[l].code, course_codes[j]) == 0)
						{
							valid_courses++;
							break;
						}
					}
				}
				if (valid_courses != nco)
				{
					printf("\nSome courses do not exist. Please add them.\n");
					printf("Enter Code for new course:\n");
					scanf("%s", code);
					for (int z = 0; z < ncourses; z++)
					{
						if (strcmp(code, courses[z].code) == 0)
						{
							printf("Code already exists.\n");
							f = 0;
							break;
						}
					}
					if (f == 1)
					{
						printf("\nEnter Name:\n");
						scanf(" %[^\n]s", name);
						printf("\nEnter Credits:\n");
						scanf("%d", &credits);
						int ninstructors;
						printf("\nEnter No.of Instructors:\n");
						scanf("%d", &ninstructors);
						instructor_ids = malloc(ninstructors * sizeof(char *));
						if (!instructor_ids)
						{
							perror("Memory allocation failed");
							exit(1);
						}
						printf("\nEnter Instructor Ids:\n");
						for (int i = 0; i < ninstructors; i++)
						{
							instructor_ids[i] = malloc(MAX * sizeof(char));
							if (!instructor_ids[i])
							{
								perror("Memory allocation failed");
								exit(1);
							}
							scanf("%s", instructor_ids[i]);
						}
						int nstudents_local;
						printf("\nEnter No.of Students:\n");
						scanf("%d", &nstudents_local);
						student_rollnos = malloc(nstudents_local * sizeof(char *));
						if (!student_rollnos)
						{
							perror("Memory allocation failed");
							exit(1);
						}
						printf("\nEnter Students Rollnos:\n");
						for (int i = 0; i < nstudents_local; i++)
						{
							student_rollnos[i] = malloc(MAX * sizeof(char));
							if (!student_rollnos[i])
							{
								perror("Memory allocation failed");
								exit(1);
							}
							scanf("%s", student_rollnos[i]);
						}
						addcourse(code, name, credits, ninstructors, instructor_ids, nstudents_local, student_rollnos);
					}
				}
			}
			break;
		case 4:
			deletestudent();
			break;
		case 5:
			modifystudent();
			break;
		default:
			printf("Enter Valid Number.\n");
			break;
		}
	}
	else if (i == 2)
	{
		switch (j)
		{
		case 1:
			print_instructors();
			break;
		case 2:
			printf("Enter ID:\n");
			scanf("%s", id);
			for (int z = 0; z < ninstructors; z++)
			{
				if (strcmp(id, instructors[z].id) == 0)
				{
					print_instructor(instructors[z]);
					m = 1;
					break;
				}
			}
			if (m == 0)
			{
				printf("Enter valid ID.\n");
			}
			break;
		case 3:
			printf("Enter ID:\n");
			scanf("%s", id);
			for (int z = 0; z < ninstructors; z++)
			{
				if (strcmp(id, instructors[z].id) == 0)
				{
					printf("Instructor already exists.\n");
					f = 0;
					break;
				}
			}
			if (f == 1)
			{
				printf("Enter Name:\n");
				scanf(" %[^\n]s", name);
				int nc;
				printf("Enter no.of Courses teaching:\n");
				scanf("%d", &nc);
				courses_codes = malloc(nc * sizeof(char *));
				if (!courses_codes)
				{
					perror("Memory allocation failed");
					exit(1);
				}
				printf("Enter Course Codes:\n");
				for (int z = 0; z < nc; z++)
				{
					courses_codes[z] = malloc(MAX * sizeof(char));
					if (!courses_codes[z])
					{
						perror("Memory allocation failed");
						exit(1);
					}
					scanf("%s", courses_codes[z]);
				}
				addinstructor(id, name, nc, courses_codes);
			}
			break;
		case 4:
			deleteinstructor();
			break;
		case 5:
			modifyinstructor();
			break;
		default:
			printf("\nEnter Valid Number.\n");
			break;
		}
	}
	else if (i == 3)
	{
		switch (j)
		{
		case 1:
			print_courses();
			break;
		case 2:
			printf("Enter Code:\n");
			scanf("%s", code);
			for (int i = 0; i < ncourses; i++)
			{
				if (strcmp(code, courses[i].code) == 0)
				{
					print_course(courses[i]);
					m = 1;
					break;
				}
			}
			if (m == 0)
			{
				printf("Enter valid Code.\n");
			}
			break;
		case 3:
			printf("Enter Code:\n");
			scanf("%s", code);
			for (int z = 0; z < ncourses; z++)
			{
				if (strcmp(code, courses[z].code) == 0)
				{
					printf("Code already exists.\n");
					f = 0;
					break;
				}
			}
			if (f == 1)
			{
				printf("\nEnter Name:\n");
				scanf(" %[^\n]s", name);
				printf("\nEnter Credits:\n");
				scanf("%d", &credits);
				int ninstructors;
				printf("\nEnter No.of Instructors:\n");
				scanf("%d", &ninstructors);
				instructor_ids = malloc(ninstructors * sizeof(char *));
				if (!instructor_ids)
				{
					perror("Memory allocation failed");
					exit(1);
				}
				printf("\nEnter Instructor Ids:\n");
				for (int i = 0; i < ninstructors; i++)
				{
					instructor_ids[i] = malloc(MAX * sizeof(char));
					if (!instructor_ids[i])
					{
						perror("Memory allocation failed");
						exit(1);
					}
					scanf("%s", instructor_ids[i]);
				}
				int nstudents_local;
				printf("\nEnter No.of Students:\n");
				scanf("%d", &nstudents_local);
				student_rollnos = malloc(nstudents_local * sizeof(char *));
				if (!student_rollnos)
				{
					perror("Memory allocation failed");
					exit(1);
				}
				printf("\nEnter Students Rollnos:\n");
				for (int i = 0; i < nstudents_local; i++)
				{
					student_rollnos[i] = malloc(MAX * sizeof(char));
					if (!student_rollnos[i])
					{
						perror("Memory allocation failed");
						exit(1);
					}
					scanf("%s", student_rollnos[i]);
				}
				addcourse(code, name, credits, ninstructors, instructor_ids, nstudents_local, student_rollnos);
			}
			break;
		case 4:
			deletecourse();
			break;
		case 5:
			modifycourse();
			break;
		default:
			printf("\nEnter Valid Number.\n");
			break;
		}
	}
	else
	{
		printf("Enter Valid Number.\n");
	}

	free_all();
	return 0;
}

// Load records from files
void load_records()
{
	load_students();
	load_instructors();
	load_courses();
}

void load_students()
{
	fps = fopen(students_file, "r");
	if (!fps)
	{
		perror("Failed to open students file");
		return;
	}
	char buf[MAX];
	struct student s;
	if (fscanf(fps, "%d", &nstudents) != 1)
	{
		fclose(fps);
		return;
	}
	students = malloc(nstudents * sizeof(struct student));
	if (!students)
	{
		perror("Memory allocation failed");
		fclose(fps);
		exit(1);
	}
	for (int i = 0; i < nstudents; i++)
	{
		fgets(buf, MAX, fps); // Consume newline
		if (fscanf(fps, "%s", s.rollno) != 1)
			break;
		fscanf(fps, " %[^\n]s", s.name);
		unsigned int branch;
		if (fscanf(fps, "%u", &branch) != 1)
			break;
		s.branch = (enum branch)branch;
		if (fscanf(fps, "%d", &s.semester) != 1)
			break;
		if (fscanf(fps, "%d", &s.ncourses) != 1)
			break;
		s.course_codes = malloc(s.ncourses * sizeof(char *));
		if (!s.course_codes)
		{
			perror("Memory allocation failed");
			fclose(fps);
			exit(1);
		}
		for (int j = 0; j < s.ncourses; j++)
		{
			s.course_codes[j] = malloc(MAX * sizeof(char));
			if (!s.course_codes[j])
			{
				perror("Memory allocation failed");
				fclose(fps);
				exit(1);
			}
			if (fscanf(fps, "%s", s.course_codes[j]) != 1)
				break;
		}
		students[i] = s;
	}
	fclose(fps);
}

void load_instructors()
{
	fpi = fopen(instructors_file, "r");
	if (!fpi)
	{
		perror("Failed to open instructors file");
		return;
	}
	char buf[MAX];
	struct instructor ins;
	if (fscanf(fpi, "%d", &ninstructors) != 1)
	{
		fclose(fpi);
		return;
	}
	instructors = malloc(ninstructors * sizeof(struct instructor));
	if (!instructors)
	{
		perror("Memory allocation failed");
		fclose(fpi);
		exit(1);
	}
	for (int i = 0; i < ninstructors; i++)
	{
		fgets(buf, MAX, fpi); // Consume newline
		if (fscanf(fpi, "%s", ins.id) != 1)
			break;
		fscanf(fpi, " %[^\n]s", ins.name);
		if (fscanf(fpi, "%d", &ins.ncourses) != 1)
			break;
		ins.courses_codes = malloc(ins.ncourses * sizeof(char *));
		if (!ins.courses_codes)
		{
			perror("Memory allocation failed");
			fclose(fpi);
			exit(1);
		}
		for (int j = 0; j < ins.ncourses; j++)
		{
			ins.courses_codes[j] = malloc(MAX * sizeof(char));
			if (!ins.courses_codes[j])
			{
				perror("Memory allocation failed");
				fclose(fpi);
				exit(1);
			}
			if (fscanf(fpi, "%s", ins.courses_codes[j]) != 1)
				break;
		}
		instructors[i] = ins;
	}
	fclose(fpi);
}

void load_courses()
{
	fpc = fopen(courses_file, "r");
	if (!fpc)
	{
		perror("Failed to open courses file");
		return;
	}
	char buf[MAX];
	struct course c;
	if (fscanf(fpc, "%d", &ncourses) != 1)
	{
		fclose(fpc);
		return;
	}
	courses = malloc(ncourses * sizeof(struct course));
	if (!courses)
	{
		perror("Memory allocation failed");
		fclose(fpc);
		exit(1);
	}
	for (int i = 0; i < ncourses; i++)
	{
		fgets(buf, MAX, fpc); // Consume newline
		if (fscanf(fpc, "%s", c.code) != 1)
			break;
		fscanf(fpc, " %[^\n]s", c.name);
		if (fscanf(fpc, "%d", &c.credits) != 1)
			break;
		if (fscanf(fpc, "%d", &c.ninstructors) != 1)
			break;
		c.instructor_ids = malloc(c.ninstructors * sizeof(char *));
		if (!c.instructor_ids)
		{
			perror("Memory allocation failed");
			fclose(fpc);
			exit(1);
		}
		for (int j = 0; j < c.ninstructors; j++)
		{
			c.instructor_ids[j] = malloc(MAX * sizeof(char));
			if (!c.instructor_ids[j])
			{
				perror("Memory allocation failed");
				fclose(fpc);
				exit(1);
			}
			if (fscanf(fpc, "%s", c.instructor_ids[j]) != 1)
				break;
		}
		if (fscanf(fpc, "%d", &c.nstudents) != 1)
			break;
		c.student_rollnos = malloc(c.nstudents * sizeof(char *));
		if (!c.student_rollnos)
		{
			perror("Memory allocation failed");
			fclose(fpc);
			exit(1);
		}
		for (int j = 0; j < c.nstudents; j++)
		{
			c.student_rollnos[j] = malloc(MAX * sizeof(char));
			if (!c.student_rollnos[j])
			{
				perror("Memory allocation failed");
				fclose(fpc);
				exit(1);
			}
			if (fscanf(fpc, "%s", c.student_rollnos[j]) != 1)
				break;
		}
		courses[i] = c;
	}
	fclose(fpc);
}

// Student functions
void print_students()
{
	printf("Students' details\n-----------------\n");
	for (int i = 0; i < nstudents; i++)
	{
		print_student(students[i]);
	}
}

void print_student(struct student s)
{
	printf("Roll no.: %s\n", s.rollno);
	printf("Name: %s\n", s.name);
	printf("Branch: %d\n", s.branch);
	printf("Semester: %d\n", s.semester);
	printf("List of courses: ");
	for (int i = 0; i < s.ncourses; i++)
	{
		printf("%s ", s.course_codes[i]);
	}
	printf("\n\n");
}

void add_student(struct student s)
{
	students = realloc(students, (nstudents + 1) * sizeof(struct student));
	if (!students)
	{
		perror("Memory allocation failed");
		exit(1);
	}
	students[nstudents] = s;
	nstudents++;
}

void addstudent(char *rollno, char *name, int b, int semester, int courses, char **course_codes)
{
	struct student s;
	strcpy(s.rollno, rollno);
	strcpy(s.name, name);
	s.branch = (enum branch)b;
	s.semester = semester;
	s.ncourses = courses;
	s.course_codes = course_codes;
	add_student(s);

	fps = fopen(students_file, "w");
	if (!fps)
	{
		perror("Failed to open students file");
		exit(1);
	}
	fprintf(fps, "%d\n", nstudents);
	for (int i = 0; i < nstudents; i++)
	{
		fprintf(fps, "\n%s\n", students[i].rollno);
		fprintf(fps, "%s\n", students[i].name);
		fprintf(fps, "%d\n", students[i].branch);
		fprintf(fps, "%d\n", students[i].semester);
		fprintf(fps, "%d\n", students[i].ncourses);
		for (int j = 0; j < students[i].ncourses; j++)
		{
			fprintf(fps, "%s ", students[i].course_codes[j]);
		}
		fprintf(fps, "\n");
	}
	fclose(fps);
}

void deletestudent()
{
	char rollno[MAX];
	int m = -1;
	printf("Enter Roll No.:\n");
	scanf("%s", rollno);
	for (int i = 0; i < nstudents; i++)
	{
		if (strcmp(rollno, students[i].rollno) == 0)
		{
			m = i; // Found the student to delete
			break;
		}
	}
	if (m == -1)
	{
		printf("Enter valid Roll no.\n");
		return;
	}

	// Free memory for the student to be deleted
	for (int j = 0; j < students[m].ncourses; j++)
	{
		free(students[m].course_codes[j]);
	}
	free(students[m].course_codes);

	// Shift remaining students
	for (int i = m; i < nstudents - 1; i++)
	{
		students[i] = students[i + 1];
	}
	nstudents--;
	students = realloc(students, nstudents * sizeof(struct student));
	if (nstudents > 0 && !students)
	{
		perror("Memory allocation failed");
		exit(1);
	}

	// Rewrite the file
	fps = fopen(students_file, "w");
	if (!fps)
	{
		perror("Failed to open students file");
		exit(1);
	}
	fprintf(fps, "%d\n", nstudents);
	for (int i = 0; i < nstudents; i++)
	{
		fprintf(fps, "\n%s\n", students[i].rollno);
		fprintf(fps, "%s\n", students[i].name);
		fprintf(fps, "%d\n", students[i].branch);
		fprintf(fps, "%d\n", students[i].semester);
		fprintf(fps, "%d\n", students[i].ncourses);
		for (int j = 0; j < students[i].ncourses; j++)
		{
			fprintf(fps, "%s ", students[i].course_codes[j]);
		}
		fprintf(fps, "\n");
	}
	fclose(fps);
}

void modifystudent()
{
	char rollno[MAX];
	int m = -1;
	printf("Enter Roll No.:");
	scanf("%s", rollno);
	for (int i = 0; i < nstudents; i++)
	{
		if (strcmp(rollno, students[i].rollno) == 0)
		{
			m = i;
			break;
		}
	}
	if (m == -1)
	{
		printf("Enter valid Roll no.\n");
		return;
	}

	int u;
	printf("Enter which detail you want to modify:\n1.Roll No.\n2.Name\n3.Branch\n4.Semester\n5.No.of Courses\n>>>>>>\n");
	scanf("%d", &u);
	char mrollno[MAX];
	char mname[MAX];
	int mb;
	int msemester;
	int mncourses;
	char **mcourse_codes;

	switch (u)
	{
	case 1:
		printf("Enter new Roll No.:\n");
		scanf("%s", mrollno);
		strcpy(students[m].rollno, mrollno);
		break;
	case 2:
		printf("Enter new Name:\n");
		scanf(" %[^\n]s", mname);
		strcpy(students[m].name, mname);
		break;
	case 3:
		printf("Enter new branch code (0=CSE, 1=EE, 2=ME):\n");
		scanf("%d", &mb);
		if (mb >= 0 && mb <= 2)
		{
			students[m].branch = (enum branch)mb;
		}
		else
		{
			printf("Invalid branch code.\n");
			return;
		}
		break;
	case 4:
		printf("Enter new Semester:\n");
		scanf("%d", &msemester);
		students[m].semester = msemester;
		break;
	case 5:
		printf("Enter new no.of courses:\n");
		scanf("%d", &mncourses);
		// Free old course codes
		for (int j = 0; j < students[m].ncourses; j++)
		{
			free(students[m].course_codes[j]);
		}
		free(students[m].course_codes);
		// Allocate new course codes
		mcourse_codes = malloc(mncourses * sizeof(char *));
		if (!mcourse_codes)
		{
			perror("Memory allocation failed");
			exit(1);
		}
		printf("Enter new Course Codes:\n");
		for (int z = 0; z < mncourses; z++)
		{
			mcourse_codes[z] = malloc(MAX * sizeof(char));
			if (!mcourse_codes[z])
			{
				perror("Memory allocation failed");
				exit(1);
			}
			scanf("%s", mcourse_codes[z]);
		}
		students[m].ncourses = mncourses;
		students[m].course_codes = mcourse_codes;
		break;
	default:
		printf("Enter valid Number.\n");
		return;
	}
	printmodifystudent();
}

void printmodifystudent()
{
	fps = fopen(students_file, "w");
	if (!fps)
	{
		perror("Failed to open students file");
		exit(1);
	}
	fprintf(fps, "%d\n", nstudents);
	for (int i = 0; i < nstudents; i++)
	{
		fprintf(fps, "\n%s\n", students[i].rollno);
		fprintf(fps, "%s\n", students[i].name);
		fprintf(fps, "%d\n", students[i].branch);
		fprintf(fps, "%d\n", students[i].semester);
		fprintf(fps, "%d\n", students[i].ncourses);
		for (int j = 0; j < students[i].ncourses; j++)
		{
			fprintf(fps, "%s ", students[i].course_codes[j]);
		}
		fprintf(fps, "\n");
	}
	fclose(fps);
}

// Instructor functions
void print_instructors()
{
	printf("Instructors' details\n-----------------\n");
	for (int i = 0; i < ninstructors; i++)
	{
		print_instructor(instructors[i]);
	}
}

void print_instructor(struct instructor i)
{
	printf("Instructor ID: %s\n", i.id);
	printf("Name: %s\n", i.name);
	printf("Number of Courses: %d\n", i.ncourses);
	printf("List of courses: ");
	for (int t = 0; t < i.ncourses; t++)
	{
		printf("%s ", i.courses_codes[t]);
	}
	printf("\n\n");
}

void add_instructor(struct instructor i)
{
	instructors = realloc(instructors, (ninstructors + 1) * sizeof(struct instructor));
	if (!instructors)
	{
		perror("Memory allocation failed");
		exit(1);
	}
	instructors[ninstructors] = i;
	ninstructors++;
}

void addinstructor(char *id, char *name, int ncourses, char **courses_codes)
{
	struct instructor i;
	strcpy(i.id, id);
	strcpy(i.name, name);
	i.ncourses = ncourses;
	i.courses_codes = courses_codes;
	add_instructor(i);

	fpi = fopen(instructors_file, "w");
	if (!fpi)
	{
		perror("Failed to open instructors file");
		exit(1);
	}
	fprintf(fpi, "%d\n", ninstructors);
	for (int i = 0; i < ninstructors; i++)
	{
		fprintf(fpi, "\n%s\n", instructors[i].id);
		fprintf(fpi, "%s\n", instructors[i].name);
		fprintf(fpi, "%d\n", instructors[i].ncourses);
		for (int j = 0; j < instructors[i].ncourses; j++)
		{
			fprintf(fpi, "%s ", instructors[i].courses_codes[j]);
		}
		fprintf(fpi, "\n");
	}
	fclose(fpi);
}

void deleteinstructor()
{
	char id[MAX];
	int m = -1;
	printf("Enter ID:\n");
	scanf("%s", id);
	for (int i = 0; i < ninstructors; i++)
	{
		if (strcmp(id, instructors[i].id) == 0)
		{
			m = i; // Found the instructor to delete
			break;
		}
	}
	if (m == -1)
	{
		printf("Enter valid ID.\n");
		return;
	}

	// Free memory for the instructor to be deleted
	for (int j = 0; j < instructors[m].ncourses; j++)
	{
		free(instructors[m].courses_codes[j]);
	}
	free(instructors[m].courses_codes);

	// Shift remaining instructors
	for (int i = m; i < ninstructors - 1; i++)
	{
		instructors[i] = instructors[i + 1];
	}
	ninstructors--;
	instructors = realloc(instructors, ninstructors * sizeof(struct instructor));
	if (ninstructors > 0 && !instructors)
	{
		perror("Memory allocation failed");
		exit(1);
	}

	// Rewrite the file
	fpi = fopen(instructors_file, "w");
	if (!fpi)
	{
		perror("Failed to open instructors file");
		exit(1);
	}
	fprintf(fpi, "%d\n", ninstructors);
	for (int i = 0; i < ninstructors; i++)
	{
		fprintf(fpi, "\n%s\n", instructors[i].id);
		fprintf(fpi, "%s\n", instructors[i].name);
		fprintf(fpi, "%d\n", instructors[i].ncourses);
		for (int j = 0; j < instructors[i].ncourses; j++)
		{
			fprintf(fpi, "%s ", instructors[i].courses_codes[j]);
		}
		fprintf(fpi, "\n");
	}
	fclose(fpi);
}

void modifyinstructor()
{
	char id[MAX];
	int m = -1;
	printf("Enter ID:");
	scanf("%s", id);
	for (int i = 0; i < ninstructors; i++)
	{
		if (strcmp(id, instructors[i].id) == 0)
		{
			m = i;
			break;
		}
	}
	if (m == -1)
	{
		printf("Enter valid ID.\n");
		return;
	}

	int u;
	printf("Enter which detail you want to modify:\n1.ID\n2.Name\n3.No.of Courses\n>>>>>>\n");
	scanf("%d", &u);
	char mid[MAX];
	char mname[MAX];
	int mncourses;
	char **mcourses_codes;

	switch (u)
	{
	case 1:
		printf("Enter new ID:\n");
		scanf("%s", mid);
		strcpy(instructors[m].id, mid);
		break;
	case 2:
		printf("Enter new Name:\n");
		scanf(" %[^\n]s", mname);
		strcpy(instructors[m].name, mname);
		break;
	case 3:
		printf("Enter new no.of courses:\n");
		scanf("%d", &mncourses);
		// Free old course codes
		for (int j = 0; j < instructors[m].ncourses; j++)
		{
			free(instructors[m].courses_codes[j]);
		}
		free(instructors[m].courses_codes);
		// Allocate new course codes
		mcourses_codes = malloc(mncourses * sizeof(char *));
		if (!mcourses_codes)
		{
			perror("Memory allocation failed");
			exit(1);
		}
		printf("Enter new Course Codes:\n");
		for (int z = 0; z < mncourses; z++)
		{
			mcourses_codes[z] = malloc(MAX * sizeof(char));
			if (!mcourses_codes[z])
			{
				perror("Memory allocation failed");
				exit(1);
			}
			scanf("%s", mcourses_codes[z]);
		}
		instructors[m].ncourses = mncourses;
		instructors[m].courses_codes = mcourses_codes;
		break;
	default:
		printf("Enter valid Number.\n");
		return;
	}
	printmodifyinstructor();
}

void printmodifyinstructor()
{
	fpi = fopen(instructors_file, "w");
	if (!fpi)
	{
		perror("Failed to open instructors file");
		exit(1);
	}
	fprintf(fpi, "%d\n", ninstructors);
	for (int i = 0; i < ninstructors; i++)
	{
		fprintf(fpi, "\n%s\n", instructors[i].id);
		fprintf(fpi, "%s\n", instructors[i].name);
		fprintf(fpi, "%d\n", instructors[i].ncourses);
		for (int j = 0; j < instructors[i].ncourses; j++)
		{
			fprintf(fpi, "%s ", instructors[i].courses_codes[j]);
		}
		fprintf(fpi, "\n");
	}
	fclose(fpi);
}

// Course functions
void print_courses()
{
	printf("Courses details\n-----------------\n");
	for (int i = 0; i < ncourses; i++)
	{
		print_course(courses[i]);
	}
}

void print_course(struct course c)
{
	printf("Code: %s\n", c.code);
	printf("Name: %s\n", c.name);
	printf("Credits: %d\n", c.credits);
	printf("Number of Instructors: %d\n", c.ninstructors);
	printf("Instructors: ");
	for (int i = 0; i < c.ninstructors; i++)
	{
		printf("%s ", c.instructor_ids[i]);
	}
	printf("\nNumber of Students: %d\n", c.nstudents);
	printf("Students: ");
	for (int i = 0; i < c.nstudents; i++)
	{
		printf("%s ", c.student_rollnos[i]);
	}
	printf("\n\n");
}

void add_course(struct course c)
{
	courses = realloc(courses, (ncourses + 1) * sizeof(struct course));
	if (!courses)
	{
		perror("Memory allocation failed");
		exit(1);
	}
	courses[ncourses] = c;
	ncourses++;
}

void addcourse(char *code, char *name, int credits, int ninstructors, char **instructor_ids, int nstudents, char **student_rollnos)
{
	struct course c;
	strcpy(c.code, code);
	strcpy(c.name, name);
	c.credits = credits;
	c.ninstructors = ninstructors;
	c.instructor_ids = instructor_ids;
	c.nstudents = nstudents;
	c.student_rollnos = student_rollnos;
	add_course(c);

	fpc = fopen(courses_file, "w");
	if (!fpc)
	{
		perror("Failed to open courses file");
		exit(1);
	}
	fprintf(fpc, "%d\n", ncourses);
	for (int i = 0; i < ncourses; i++)
	{
		fprintf(fpc, "\n%s\n", courses[i].code);
		fprintf(fpc, "%s\n", courses[i].name);
		fprintf(fpc, "%d\n", courses[i].credits);
		fprintf(fpc, "%d\n", courses[i].ninstructors);
		for (int j = 0; j < courses[i].ninstructors; j++)
		{
			fprintf(fpc, "%s ", courses[i].instructor_ids[j]);
		}
		fprintf(fpc, "\n%d\n", courses[i].nstudents);
		for (int j = 0; j < courses[i].nstudents; j++)
		{
			fprintf(fpc, "%s ", courses[i].student_rollnos[j]);
		}
		fprintf(fpc, "\n");
	}
	fclose(fpc);
}

void deletecourse()
{
	char code[MAX];
	int m = -1;
	printf("Enter Code:\n");
	scanf("%s", code);
	for (int i = 0; i < ncourses; i++)
	{
		if (strcmp(code, courses[i].code) == 0)
		{
			m = i; // Found the course to delete
			break;
		}
	}
	if (m == -1)
	{
		printf("Enter valid Code.\n");
		return;
	}

	// Free memory for the course to be deleted
	for (int j = 0; j < courses[m].ninstructors; j++)
	{
		free(courses[m].instructor_ids[j]);
	}
	free(courses[m].instructor_ids);
	for (int j = 0; j < courses[m].nstudents; j++)
	{
		free(courses[m].student_rollnos[j]);
	}
	free(courses[m].student_rollnos);

	// Shift remaining courses
	for (int i = m; i < ncourses - 1; i++)
	{
		courses[i] = courses[i + 1];
	}
	ncourses--;
	courses = realloc(courses, ncourses * sizeof(struct course));
	if (ncourses > 0 && !courses)
	{
		perror("Memory allocation failed");
		exit(1);
	}

	// Rewrite the file
	fpc = fopen(courses_file, "w");
	if (!fpc)
	{
		perror("Failed to open courses file");
		exit(1);
	}
	fprintf(fpc, "%d\n", ncourses);
	for (int i = 0; i < ncourses; i++)
	{
		fprintf(fpc, "\n%s\n", courses[i].code);
		fprintf(fpc, "%s\n", courses[i].name);
		fprintf(fpc, "%d\n", courses[i].credits);
		fprintf(fpc, "%d\n", courses[i].ninstructors);
		for (int j = 0; j < courses[i].ninstructors; j++)
		{
			fprintf(fpc, "%s ", courses[i].instructor_ids[j]);
		}
		fprintf(fpc, "\n%d\n", courses[i].nstudents);
		for (int j = 0; j < courses[i].nstudents; j++)
		{
			fprintf(fpc, "%s ", courses[i].student_rollnos[j]);
		}
		fprintf(fpc, "\n");
	}
	fclose(fpc);
}

void modifycourse()
{
	char code[MAX];
	int m = -1;
	printf("Enter Code:");
	scanf("%s", code);
	for (int i = 0; i < ncourses; i++)
	{
		if (strcmp(code, courses[i].code) == 0)
		{
			m = i;
			break;
		}
	}
	if (m == -1)
	{
		printf("Enter valid Code.\n");
		return;
	}

	int u;
	printf("Enter which detail you want to modify:\n1.Code\n2.Name\n3.Credits\n4.No.of Instructors\n5.No.of Students\n>>>>>>\n");
	scanf("%d", &u);
	char mcode[MAX];
	char mname[MAX];
	int mcredits;
	int mninstructors;
	char **minstructor_ids;
	int mnstudents;
	char **mstudent_rollnos;

	switch (u)
	{
	case 1:
		printf("Enter new Code:\n");
		scanf("%s", mcode);
		strcpy(courses[m].code, mcode);
		break;
	case 2:
		printf("Enter new Name:\n");
		scanf(" %[^\n]s", mname);
		strcpy(courses[m].name, mname);
		break;
	case 3:
		printf("Enter new Credits:\n");
		scanf("%d", &mcredits);
		courses[m].credits = mcredits;
		break;
	case 4:
		printf("Enter new no.of instructors:\n");
		scanf("%d", &mninstructors);
		// Free old instructor IDs
		for (int j = 0; j < courses[m].ninstructors; j++)
		{
			free(courses[m].instructor_ids[j]);
		}
		free(courses[m].instructor_ids);
		// Allocate new instructor IDs
		minstructor_ids = malloc(mninstructors * sizeof(char *));
		if (!minstructor_ids)
		{
			perror("Memory allocation failed");
			exit(1);
		}
		printf("Enter new Instructor IDs:\n");
		for (int z = 0; z < mninstructors; z++)
		{
			minstructor_ids[z] = malloc(MAX * sizeof(char));
			if (!minstructor_ids[z])
			{
				perror("Memory allocation failed");
				exit(1);
			}
			scanf("%s", minstructor_ids[z]);
		}
		courses[m].ninstructors = mninstructors;
		courses[m].instructor_ids = minstructor_ids;
		break;
	case 5:
		printf("Enter new no.of students:\n");
		scanf("%d", &mnstudents);
		// Free old student roll numbers
		for (int j = 0; j < courses[m].nstudents; j++)
		{
			free(courses[m].student_rollnos[j]);
		}
		free(courses[m].student_rollnos);
		// Allocate new student roll numbers
		mstudent_rollnos = malloc(mnstudents * sizeof(char *));
		if (!mstudent_rollnos)
		{
			perror("Memory allocation failed");
			exit(1);
		}
		printf("Enter new Student Roll Nos:\n");
		for (int z = 0; z < mnstudents; z++)
		{
			mstudent_rollnos[z] = malloc(MAX * sizeof(char));
			if (!mstudent_rollnos[z])
			{
				perror("Memory allocation failed");
				exit(1);
			}
			scanf("%s", mstudent_rollnos[z]);
		}
		courses[m].nstudents = mnstudents;
		courses[m].student_rollnos = mstudent_rollnos;
		break;
	default:
		printf("Enter valid Number.\n");
		return;
	}
	printmodifycourse();
}

void printmodifycourse()
{
	fpc = fopen(courses_file, "w");
	if (!fpc)
	{
		perror("Failed to open courses file");
		exit(1);
	}
	fprintf(fpc, "%d\n", ncourses);
	for (int i = 0; i < ncourses; i++)
	{
		fprintf(fpc, "\n%s\n", courses[i].code);
		fprintf(fpc, "%s\n", courses[i].name);
		fprintf(fpc, "%d\n", courses[i].credits);
		fprintf(fpc, "%d\n", courses[i].ninstructors);
		for (int j = 0; j < courses[i].ninstructors; j++)
		{
			fprintf(fpc, "%s ", courses[i].instructor_ids[j]);
		}
		fprintf(fpc, "\n%d\n", courses[i].nstudents);
		for (int j = 0; j < courses[i].nstudents; j++)
		{
			fprintf(fpc, "%s ", courses[i].student_rollnos[j]);
		}
		fprintf(fpc, "\n");
	}
	fclose(fpc);
}

// Free all allocated memory
void free_all()
{
	for (int i = 0; i < nstudents; i++)
	{
		for (int j = 0; j < students[i].ncourses; j++)
		{
			free(students[i].course_codes[j]);
		}
		free(students[i].course_codes);
	}
	free(students);

	for (int i = 0; i < ninstructors; i++)
	{
		for (int j = 0; j < instructors[i].ncourses; j++)
		{
			free(instructors[i].courses_codes[j]);
		}
		free(instructors[i].courses_codes);
	}
	free(instructors);

	for (int i = 0; i < ncourses; i++)
	{
		for (int j = 0; j < courses[i].ninstructors; j++)
		{
			free(courses[i].instructor_ids[j]);
		}
		free(courses[i].instructor_ids);
		for (int j = 0; j < courses[i].nstudents; j++)
		{
			free(courses[i].student_rollnos[j]);
		}
		free(courses[i].student_rollnos);
	}
	free(courses);
}