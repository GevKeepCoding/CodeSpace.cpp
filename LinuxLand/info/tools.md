
## Address sanitizer
### Setup
on bash
```bash
# Compilation with address sanitizer flags
gcc -fsanitize=address,undefined -g -O1 -o app app.c
```
on gradle
```gradle
        binaries.configureEach {
            def cppCompile = compileTask.get()
            if (toolChain instanceof GccCompatibleToolChain) {
                cppCompile.compilerArgs.add "-Wall"
                cppCompile.includes("${root_dir}/build/common/src/main/public/")
            }

            cppCompile.compilerArgs.add "-fsanitize=address"
            cppCompile.compilerArgs.add "-fno-omit-frame-pointer"
 
            def linkTask = linkTask.get()
            linkTask.linkerArgs.add "-L/tools/dev/lnx64/gcc-8.3.0/lib64"
            linkTask.linkerArgs.add "-Wl,--no-undefined"
            linkTask.linkerArgs.add "-fsanitize=address"
        }
```        

### Run

```bash
#address sanitizer is mostly useful for debugging memory issues like buffer overflows, use after free or memory leaks or
export ASAN_OPTIONS="detect_leaks=1:halt_on_error=0:suppressions=/proj/xedhdstaff1/garakely/nobkup/tools/sanitizer/asan_suppressions.txt:log_path=asan_errors.log"
LD_PRELOAD=$(gcc -print-file-name=libasan.so):$(gcc -print-file-name=libubsan.so) ./app args
```

### Suppressions
```suppression.txt
# Suppressions for Address Sanitizer

# 1) Silence the specific leak produced by our helper() function  
#    (any stack that contains the symbol ‘helper’ will be suppressed).  
leak:helper
leak:HATAttrSpec::addEnumRange  
  
# 2) Silence every leak coming from iconv inside glibc  
#    (a well-known intentional leak).  
leak:__gconv_open  
  
# 3) Hide leaks in all Boost.System code (library name match).  
leak:libboost_system  
  
# 4) Suppress an occasional use-after-free ASan reports  
#    when we dlopen NVIDIA’s GL driver.  
addr:libnvidia-glcore  
  
# 5) For bigger projects using Qt – ignore the “leak” that  
#    qRegisterResourceData() keeps around forever.  
leak:qRegisterResourceData  
```