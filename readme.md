/var/log/wtmp
/var/run/utmp
/var/log/lastlog

`zap -a username -t devicename -d mmddyy -i 127.0.0.1 192.168.0.1`
- a 옵션: account (username)을 선택하여 삭제하도록 함
- t 옵션: 접속했던 device name 선택하여 편집하도록 함
- h 옵션: 접속했던 host 선택하여 편집하도록 함
- d 옵션: 년월일시를 지정하여 특정 시간대의 접속기록을 편집할 수 있도록 함)  

`zap -R -a username1 username2 -t devicename1 devicename2 -d ddmmyy1 ddmmyy2 -i 127.0.0.1 192.168.0.1`
- R 옵션을 주어서 선택한 기록을 다른 내용으로 대체할 수 있도록 편집기능을 구현한다. (삭제가 아닌 변조 기능)

gcc
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:hsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.3.0-17ubuntu1~20.04' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-HskZEa/gcc-9-9.3.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)
