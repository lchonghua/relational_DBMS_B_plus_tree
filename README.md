This project implements a simple database management system with B+ tree indexing for handling student records. 
Each student record contains last name, first name and a confidential information field. 

The program is able to:
1. INSERT new student records into a possibly empty table;
2. SEARCH for the record of a given student and print it out;
3. LIST all students names in the appropriate order;
4. UPDATE a given student record;
5. DELETE a given student record;
6. SNAPSHOT: 
    - print the number of records in the table; 
    - print the depth of the B+ tree;
    - print the first and last key of all the B+ tree nodes.

Sample input data:
------------------
  *INSERT
  
  Walker, Chris
  
  Toleti, Srinivas
  
  WALKER, CHRIS
  
  Rawlinson, Edward M
  
  Mason, Eric J
  
  Wu, Chang‐Yu
  
  *SNAPSHOT
  
  *SEARCH
  
  rawlinson, edward m
  
  Wong, Qun
  
  *INSERT
  
  Chiang, Mark
  
  *DELETE
  
  Long, Robert
  
  *SNAPSHOT
  
  *UPDATE
  
  WALKER, CHRIS
  
  New project.

Note that All keywords are preceded by an asterisk. The information to be inserted in the confidential information field 
of a student record only appear in input lines describing updates and will always start on a new line. 

Reference for implementing B+ tree: B+ Tree Implementation * Copyright (C) 2010  Amittai Aviram  http://www.amittai.com
