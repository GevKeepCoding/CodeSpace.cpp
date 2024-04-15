There are limits to the file size parameter:

  1. **Operating System Limits**: Different file systems have different maximum file size limits (e.g., NTFS: 16TB-256TB, ext4: 16TB)

  2. **Architecture Limits**:
    - On 32-bit systems: ~2GB practical limit
    - On 64-bit systems: Much larger but still limited by virtual address space

  3. **Available Disk Space**: Cannot exceed free disk space

4. **Virtual Memory Constraints**: System must have enough virtual memory to map the file
     - **Checking virtual memory limits**:
         - On Linux: `cat /proc/meminfo` (look for CommitLimit and CommittedAS)
         - On Windows: Task Manager > Performance tab > Memory > "Virtual Memory" details
         - On macOS: `sysctl vm.swapusage` or Activity Monitor > Memory tab
     - Use `ulimit -v` on Unix-like systems to see process virtual memory limits

  If these limits are exceeded, you might get exceptions like:
  - `boost::interprocess::interprocess_exception`
  - `std::bad_alloc`
  - System-specific errors

  For extremely large mappings, consider using multiple smaller mapped segments instead.