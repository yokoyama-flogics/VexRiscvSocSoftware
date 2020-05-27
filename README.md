# VexRiscvSocSoftware for modified Murax and Mini-Briey

Original GitHub site is https://github.com/SpinalHDL/VexRiscvSocSoftware .

I have prepared (or modified original demo code) for my own TinyFPGA BX
demo code for Murax and Mini-Briey.

Please refer this [VexRiscv Murax (w/o XiP) and Mini-Briey (with XiP)
ports for TinyFPGA-BX](https://github.com/yokoyama-flogics/VexRiscv)
repository for detail.

**The following describes the original SpinalHDL/VexRiscvSocSoftware.**


Need the prebuild GCC from https://www.sifive.com/products/tools/   =>   SiFive GNU Embedded Toolchain

The makefiles are expecting to find this prebuild version in /opt/riscv/__contentOfThisPreBuild__

```sh
wget https://static.dev.sifive.com/dev-tools/riscv64-unknown-elf-gcc-20171231-x86_64-linux-centos6.tar.gz
tar -xzvf riscv64-unknown-elf-gcc-20171231-x86_64-linux-centos6.tar.gz
sudo mv riscv64-unknown-elf-gcc-20171231-x86_64-linux-centos6 /opt/riscv64-unknown-elf-gcc-20171231-x86_64-linux-centos6
sudo mv /opt/riscv64-unknown-elf-gcc-20171231-x86_64-linux-centos6 /opt/riscv
echo 'export PATH=/opt/riscv/bin:$PATH' >> ~/.bashrc 
```
