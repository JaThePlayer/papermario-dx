
.section .text8006E870, "ax"

func_8006E870:
/* 049C70 8006E870 27BDFFF8 */  addiu $sp, $sp, -8
/* 049C74 8006E874 00E0402D */  move  $t0, $a3
/* 049C78 8006E878 00C0482D */  move  $t1, $a2
/* 049C7C 8006E87C 00A0582D */  move  $t3, $a1
/* 049C80 8006E880 0080502D */  move  $t2, $a0
/* 049C84 8006E884 152000A6 */  bnez  $t1, .L8006EB20
/* 049C88 8006E888 03A0C82D */   move  $t9, $sp
/* 049C8C 8006E88C 0148102B */  sltu  $v0, $t2, $t0
/* 049C90 8006E890 1040001D */  beqz  $v0, .L8006E908
/* 049C94 8006E894 3402FFFF */   ori   $v0, $zero, 0xffff
/* 049C98 8006E898 0048102B */  sltu  $v0, $v0, $t0
/* 049C9C 8006E89C 14400005 */  bnez  $v0, .L8006E8B4
/* 049CA0 8006E8A0 00E0302D */   move  $a2, $a3
/* 049CA4 8006E8A4 2D020100 */  sltiu $v0, $t0, 0x100
/* 049CA8 8006E8A8 38420001 */  xori  $v0, $v0, 1
/* 049CAC 8006E8AC 0801BA33 */  j     func_8006E8CC
/* 049CB0 8006E8B0 000218C0 */   sll   $v1, $v0, 3

.L8006E8B4:
/* 049CB4 8006E8B4 3C0200FF */  lui   $v0, 0xff
/* 049CB8 8006E8B8 3442FFFF */  ori   $v0, $v0, 0xffff
/* 049CBC 8006E8BC 0048102B */  sltu  $v0, $v0, $t0
/* 049CC0 8006E8C0 14400002 */  bnez  $v0, .L8006E8CC
/* 049CC4 8006E8C4 24030018 */   addiu $v1, $zero, 0x18
/* 049CC8 8006E8C8 24030010 */  addiu $v1, $zero, 0x10
func_8006E8CC:
.L8006E8CC:
/* 049CCC 8006E8CC 00661006 */  srlv  $v0, $a2, $v1
/* 049CD0 8006E8D0 3C01800A */  lui   $at, 0x800a
/* 049CD4 8006E8D4 00220821 */  addu  $at, $at, $v0
/* 049CD8 8006E8D8 9022A140 */  lbu   $v0, -0x5ec0($at)
/* 049CDC 8006E8DC 24040020 */  addiu $a0, $zero, 0x20
/* 049CE0 8006E8E0 00431021 */  addu  $v0, $v0, $v1
/* 049CE4 8006E8E4 00826823 */  subu  $t5, $a0, $v0
/* 049CE8 8006E8E8 11A0005B */  beqz  $t5, .L8006EA58
/* 049CEC 8006E8EC 01AA1804 */   sllv  $v1, $t2, $t5
/* 049CF0 8006E8F0 01A84004 */  sllv  $t0, $t0, $t5
/* 049CF4 8006E8F4 008D1023 */  subu  $v0, $a0, $t5
/* 049CF8 8006E8F8 004B1006 */  srlv  $v0, $t3, $v0
/* 049CFC 8006E8FC 00625025 */  or    $t2, $v1, $v0
/* 049D00 8006E900 0801BA96 */  j     func_8006EA58
/* 049D04 8006E904 01AB5804 */   sllv  $t3, $t3, $t5

.L8006E908:
/* 049D08 8006E908 15000008 */  bnez  $t0, .L8006E92C
/* 049D0C 8006E90C 0048102B */   sltu  $v0, $v0, $t0
/* 049D10 8006E910 24020001 */  addiu $v0, $zero, 1
/* 049D14 8006E914 15200002 */  bnez  $t1, .L8006E920
/* 049D18 8006E918 0049001B */   divu  $zero, $v0, $t1
/* 049D1C 8006E91C 0007000D */  break 7
.L8006E920:
/* 049D20 8006E920 00004012 */  mflo  $t0
/* 049D24 8006E924 3402FFFF */  ori   $v0, $zero, 0xffff
/* 049D28 8006E928 0048102B */  sltu  $v0, $v0, $t0
.L8006E92C:
/* 049D2C 8006E92C 14400005 */  bnez  $v0, .L8006E944
/* 049D30 8006E930 0100202D */   move  $a0, $t0
/* 049D34 8006E934 2D020100 */  sltiu $v0, $t0, 0x100
/* 049D38 8006E938 38420001 */  xori  $v0, $v0, 1
/* 049D3C 8006E93C 0801BA57 */  j     func_8006E95C
/* 049D40 8006E940 000218C0 */   sll   $v1, $v0, 3

.L8006E944:
/* 049D44 8006E944 3C0200FF */  lui   $v0, 0xff
/* 049D48 8006E948 3442FFFF */  ori   $v0, $v0, 0xffff
/* 049D4C 8006E94C 0048102B */  sltu  $v0, $v0, $t0
/* 049D50 8006E950 14400002 */  bnez  $v0, .L8006E95C
/* 049D54 8006E954 24030018 */   addiu $v1, $zero, 0x18
/* 049D58 8006E958 24030010 */  addiu $v1, $zero, 0x10
func_8006E95C:
.L8006E95C:
/* 049D5C 8006E95C 00641006 */  srlv  $v0, $a0, $v1
/* 049D60 8006E960 3C01800A */  lui   $at, 0x800a
/* 049D64 8006E964 00220821 */  addu  $at, $at, $v0
/* 049D68 8006E968 9022A140 */  lbu   $v0, -0x5ec0($at)
/* 049D6C 8006E96C 00431021 */  addu  $v0, $v0, $v1
/* 049D70 8006E970 24030020 */  addiu $v1, $zero, 0x20
/* 049D74 8006E974 00626823 */  subu  $t5, $v1, $v0
/* 049D78 8006E978 15A00003 */  bnez  $t5, .L8006E988
/* 049D7C 8006E97C 006DC023 */   subu  $t8, $v1, $t5
/* 049D80 8006E980 0801BA96 */  j     func_8006EA58
/* 049D84 8006E984 01485023 */   subu  $t2, $t2, $t0

.L8006E988:
/* 049D88 8006E988 01A84004 */  sllv  $t0, $t0, $t5
/* 049D8C 8006E98C 030A2006 */  srlv  $a0, $t2, $t8
/* 049D90 8006E990 01AA1804 */  sllv  $v1, $t2, $t5
/* 049D94 8006E994 030B1006 */  srlv  $v0, $t3, $t8
/* 049D98 8006E998 00625025 */  or    $t2, $v1, $v0
/* 049D9C 8006E99C 00082C02 */  srl   $a1, $t0, 0x10
/* 049DA0 8006E9A0 14A00002 */  bnez  $a1, .L8006E9AC
/* 049DA4 8006E9A4 0085001B */   divu  $zero, $a0, $a1
/* 049DA8 8006E9A8 0007000D */  break 7
.L8006E9AC:
/* 049DAC 8006E9AC 00001012 */  mflo  $v0
/* 049DB0 8006E9B0 00001810 */  mfhi  $v1
/* 049DB4 8006E9B4 3106FFFF */  andi  $a2, $t0, 0xffff
/* 049DB8 8006E9B8 00000000 */  nop   
/* 049DBC 8006E9BC 00460018 */  mult  $v0, $a2
/* 049DC0 8006E9C0 00031C00 */  sll   $v1, $v1, 0x10
/* 049DC4 8006E9C4 000A1402 */  srl   $v0, $t2, 0x10
/* 049DC8 8006E9C8 00621825 */  or    $v1, $v1, $v0
/* 049DCC 8006E9CC 00002012 */  mflo  $a0
/* 049DD0 8006E9D0 0064102B */  sltu  $v0, $v1, $a0
/* 049DD4 8006E9D4 10400008 */  beqz  $v0, .L8006E9F8
/* 049DD8 8006E9D8 01AB5804 */   sllv  $t3, $t3, $t5
/* 049DDC 8006E9DC 00681821 */  addu  $v1, $v1, $t0
/* 049DE0 8006E9E0 0068102B */  sltu  $v0, $v1, $t0
/* 049DE4 8006E9E4 54400005 */  bnezl $v0, .L8006E9FC
/* 049DE8 8006E9E8 00641823 */   subu  $v1, $v1, $a0
/* 049DEC 8006E9EC 0064102B */  sltu  $v0, $v1, $a0
/* 049DF0 8006E9F0 54400001 */  bnezl $v0, .L8006E9F8
/* 049DF4 8006E9F4 00681821 */   addu  $v1, $v1, $t0
.L8006E9F8:
/* 049DF8 8006E9F8 00641823 */  subu  $v1, $v1, $a0
.L8006E9FC:
/* 049DFC 8006E9FC 14A00002 */  bnez  $a1, .L8006EA08
/* 049E00 8006EA00 0065001B */   divu  $zero, $v1, $a1
/* 049E04 8006EA04 0007000D */  break 7
.L8006EA08:
/* 049E08 8006EA08 00001012 */  mflo  $v0
/* 049E0C 8006EA0C 00001810 */  mfhi  $v1
/* 049E10 8006EA10 00000000 */  nop   
/* 049E14 8006EA14 00000000 */  nop   
/* 049E18 8006EA18 00460018 */  mult  $v0, $a2
/* 049E1C 8006EA1C 00031C00 */  sll   $v1, $v1, 0x10
/* 049E20 8006EA20 3142FFFF */  andi  $v0, $t2, 0xffff
/* 049E24 8006EA24 00621825 */  or    $v1, $v1, $v0
/* 049E28 8006EA28 00002012 */  mflo  $a0
/* 049E2C 8006EA2C 0064102B */  sltu  $v0, $v1, $a0
/* 049E30 8006EA30 50400009 */  beql  $v0, $zero, .L8006EA58
/* 049E34 8006EA34 00645023 */   subu  $t2, $v1, $a0
/* 049E38 8006EA38 00681821 */  addu  $v1, $v1, $t0
/* 049E3C 8006EA3C 0068102B */  sltu  $v0, $v1, $t0
/* 049E40 8006EA40 14400005 */  bnez  $v0, .L8006EA58
/* 049E44 8006EA44 00645023 */   subu  $t2, $v1, $a0
/* 049E48 8006EA48 0064102B */  sltu  $v0, $v1, $a0
/* 049E4C 8006EA4C 54400001 */  bnezl $v0, .L8006EA54
/* 049E50 8006EA50 00681821 */   addu  $v1, $v1, $t0
.L8006EA54:
/* 049E54 8006EA54 00645023 */  subu  $t2, $v1, $a0
func_8006EA58:
.L8006EA58:
/* 049E58 8006EA58 00082C02 */  srl   $a1, $t0, 0x10
/* 049E5C 8006EA5C 14A00002 */  bnez  $a1, .L8006EA68
/* 049E60 8006EA60 0145001B */   divu  $zero, $t2, $a1
/* 049E64 8006EA64 0007000D */  break 7
.L8006EA68:
/* 049E68 8006EA68 00001012 */  mflo  $v0
/* 049E6C 8006EA6C 00001810 */  mfhi  $v1
/* 049E70 8006EA70 3106FFFF */  andi  $a2, $t0, 0xffff
/* 049E74 8006EA74 00000000 */  nop   
/* 049E78 8006EA78 00460018 */  mult  $v0, $a2
/* 049E7C 8006EA7C 00031C00 */  sll   $v1, $v1, 0x10
/* 049E80 8006EA80 000B1402 */  srl   $v0, $t3, 0x10
/* 049E84 8006EA84 00621825 */  or    $v1, $v1, $v0
/* 049E88 8006EA88 00002012 */  mflo  $a0
/* 049E8C 8006EA8C 0064102B */  sltu  $v0, $v1, $a0
/* 049E90 8006EA90 50400009 */  beql  $v0, $zero, .L8006EAB8
/* 049E94 8006EA94 00641823 */   subu  $v1, $v1, $a0
/* 049E98 8006EA98 00681821 */  addu  $v1, $v1, $t0
/* 049E9C 8006EA9C 0068102B */  sltu  $v0, $v1, $t0
/* 049EA0 8006EAA0 54400005 */  bnezl $v0, .L8006EAB8
/* 049EA4 8006EAA4 00641823 */   subu  $v1, $v1, $a0
/* 049EA8 8006EAA8 0064102B */  sltu  $v0, $v1, $a0
/* 049EAC 8006EAAC 54400001 */  bnezl $v0, .L8006EAB4
/* 049EB0 8006EAB0 00681821 */   addu  $v1, $v1, $t0
.L8006EAB4:
/* 049EB4 8006EAB4 00641823 */  subu  $v1, $v1, $a0
.L8006EAB8:
/* 049EB8 8006EAB8 14A00002 */  bnez  $a1, .L8006EAC4
/* 049EBC 8006EABC 0065001B */   divu  $zero, $v1, $a1
/* 049EC0 8006EAC0 0007000D */  break 7
.L8006EAC4:
/* 049EC4 8006EAC4 00001012 */  mflo  $v0
/* 049EC8 8006EAC8 00001810 */  mfhi  $v1
/* 049ECC 8006EACC 00000000 */  nop   
/* 049ED0 8006EAD0 00000000 */  nop   
/* 049ED4 8006EAD4 00460018 */  mult  $v0, $a2
/* 049ED8 8006EAD8 00031C00 */  sll   $v1, $v1, 0x10
/* 049EDC 8006EADC 3162FFFF */  andi  $v0, $t3, 0xffff
/* 049EE0 8006EAE0 00621825 */  or    $v1, $v1, $v0
/* 049EE4 8006EAE4 00002012 */  mflo  $a0
/* 049EE8 8006EAE8 0064102B */  sltu  $v0, $v1, $a0
/* 049EEC 8006EAEC 10400007 */  beqz  $v0, .L8006EB0C
/* 049EF0 8006EAF0 00000000 */   nop   
/* 049EF4 8006EAF4 00681821 */  addu  $v1, $v1, $t0
/* 049EF8 8006EAF8 0068102B */  sltu  $v0, $v1, $t0
/* 049EFC 8006EAFC 14400003 */  bnez  $v0, .L8006EB0C
/* 049F00 8006EB00 0064102B */   sltu  $v0, $v1, $a0
/* 049F04 8006EB04 54400001 */  bnezl $v0, .L8006EB0C
/* 049F08 8006EB08 00681821 */   addu  $v1, $v1, $t0
.L8006EB0C:
/* 049F0C 8006EB0C 13200087 */  beqz  $t9, .L8006ED2C
/* 049F10 8006EB10 00645823 */   subu  $t3, $v1, $a0
/* 049F14 8006EB14 01AB7806 */  srlv  $t7, $t3, $t5
/* 049F18 8006EB18 0801BB49 */  j     func_8006ED24
/* 049F1C 8006EB1C 0000702D */   move  $t6, $zero

.L8006EB20:
/* 049F20 8006EB20 0149102B */  sltu  $v0, $t2, $t1
/* 049F24 8006EB24 10400007 */  beqz  $v0, .L8006EB44
/* 049F28 8006EB28 3402FFFF */   ori   $v0, $zero, 0xffff
/* 049F2C 8006EB2C 00A0782D */  move  $t7, $a1
/* 049F30 8006EB30 0140702D */  move  $t6, $t2
/* 049F34 8006EB34 AFAE0000 */  sw    $t6, ($sp)
/* 049F38 8006EB38 AFAF0004 */  sw    $t7, 4($sp)
/* 049F3C 8006EB3C 0801BB4B */  j     func_8006ED2C
/* 049F40 8006EB40 00000000 */   nop   

.L8006EB44:
/* 049F44 8006EB44 0049102B */  sltu  $v0, $v0, $t1
/* 049F48 8006EB48 14400005 */  bnez  $v0, .L8006EB60
/* 049F4C 8006EB4C 0120202D */   move  $a0, $t1
/* 049F50 8006EB50 2D220100 */  sltiu $v0, $t1, 0x100
/* 049F54 8006EB54 38420001 */  xori  $v0, $v0, 1
/* 049F58 8006EB58 0801BADE */  j     func_8006EB78
/* 049F5C 8006EB5C 000218C0 */   sll   $v1, $v0, 3

.L8006EB60:
/* 049F60 8006EB60 3C0200FF */  lui   $v0, 0xff
/* 049F64 8006EB64 3442FFFF */  ori   $v0, $v0, 0xffff
/* 049F68 8006EB68 0049102B */  sltu  $v0, $v0, $t1
/* 049F6C 8006EB6C 14400002 */  bnez  $v0, .L8006EB78
/* 049F70 8006EB70 24030018 */   addiu $v1, $zero, 0x18
/* 049F74 8006EB74 24030010 */  addiu $v1, $zero, 0x10
func_8006EB78:
.L8006EB78:
/* 049F78 8006EB78 00641006 */  srlv  $v0, $a0, $v1
/* 049F7C 8006EB7C 3C01800A */  lui   $at, 0x800a
/* 049F80 8006EB80 00220821 */  addu  $at, $at, $v0
/* 049F84 8006EB84 9022A140 */  lbu   $v0, -0x5ec0($at)
/* 049F88 8006EB88 00431021 */  addu  $v0, $v0, $v1
/* 049F8C 8006EB8C 24030020 */  addiu $v1, $zero, 0x20
/* 049F90 8006EB90 00626823 */  subu  $t5, $v1, $v0
/* 049F94 8006EB94 15A0000F */  bnez  $t5, .L8006EBD4
/* 049F98 8006EB98 006DC023 */   subu  $t8, $v1, $t5
/* 049F9C 8006EB9C 012A102B */  sltu  $v0, $t1, $t2
/* 049FA0 8006EBA0 14400004 */  bnez  $v0, .L8006EBB4
/* 049FA4 8006EBA4 01682023 */   subu  $a0, $t3, $t0
/* 049FA8 8006EBA8 0168102B */  sltu  $v0, $t3, $t0
/* 049FAC 8006EBAC 14400005 */  bnez  $v0, .L8006EBC4
/* 049FB0 8006EBB0 00000000 */   nop   
.L8006EBB4:
/* 049FB4 8006EBB4 01491823 */  subu  $v1, $t2, $t1
/* 049FB8 8006EBB8 0164102B */  sltu  $v0, $t3, $a0
/* 049FBC 8006EBBC 00625023 */  subu  $t2, $v1, $v0
/* 049FC0 8006EBC0 0080582D */  move  $t3, $a0
.L8006EBC4:
/* 049FC4 8006EBC4 13200059 */  beqz  $t9, .L8006ED2C
/* 049FC8 8006EBC8 0160782D */   move  $t7, $t3
/* 049FCC 8006EBCC 0801BB49 */  j     func_8006ED24
/* 049FD0 8006EBD0 0140702D */   move  $t6, $t2

.L8006EBD4:
/* 049FD4 8006EBD4 01A91804 */  sllv  $v1, $t1, $t5
/* 049FD8 8006EBD8 03081006 */  srlv  $v0, $t0, $t8
/* 049FDC 8006EBDC 00624825 */  or    $t1, $v1, $v0
/* 049FE0 8006EBE0 01A84004 */  sllv  $t0, $t0, $t5
/* 049FE4 8006EBE4 030A2006 */  srlv  $a0, $t2, $t8
/* 049FE8 8006EBE8 01AA1804 */  sllv  $v1, $t2, $t5
/* 049FEC 8006EBEC 030B1006 */  srlv  $v0, $t3, $t8
/* 049FF0 8006EBF0 00625025 */  or    $t2, $v1, $v0
/* 049FF4 8006EBF4 00093402 */  srl   $a2, $t1, 0x10
/* 049FF8 8006EBF8 14C00002 */  bnez  $a2, .L8006EC04
/* 049FFC 8006EBFC 0086001B */   divu  $zero, $a0, $a2
/* 04A000 8006EC00 0007000D */  break 7
.L8006EC04:
/* 04A004 8006EC04 00003812 */  mflo  $a3
/* 04A008 8006EC08 00001810 */  mfhi  $v1
/* 04A00C 8006EC0C 312CFFFF */  andi  $t4, $t1, 0xffff
/* 04A010 8006EC10 00000000 */  nop   
/* 04A014 8006EC14 00EC0018 */  mult  $a3, $t4
/* 04A018 8006EC18 000A1402 */  srl   $v0, $t2, 0x10
/* 04A01C 8006EC1C 00031C00 */  sll   $v1, $v1, 0x10
/* 04A020 8006EC20 00621825 */  or    $v1, $v1, $v0
/* 04A024 8006EC24 00002812 */  mflo  $a1
/* 04A028 8006EC28 0065102B */  sltu  $v0, $v1, $a1
/* 04A02C 8006EC2C 1040000A */  beqz  $v0, .L8006EC58
/* 04A030 8006EC30 01AB5804 */   sllv  $t3, $t3, $t5
/* 04A034 8006EC34 00691821 */  addu  $v1, $v1, $t1
/* 04A038 8006EC38 0069102B */  sltu  $v0, $v1, $t1
/* 04A03C 8006EC3C 14400006 */  bnez  $v0, .L8006EC58
/* 04A040 8006EC40 24E7FFFF */   addiu $a3, $a3, -1
/* 04A044 8006EC44 0065102B */  sltu  $v0, $v1, $a1
/* 04A048 8006EC48 50400004 */  beql  $v0, $zero, .L8006EC5C
/* 04A04C 8006EC4C 00651823 */   subu  $v1, $v1, $a1
/* 04A050 8006EC50 24E7FFFF */  addiu $a3, $a3, -1
/* 04A054 8006EC54 00691821 */  addu  $v1, $v1, $t1
.L8006EC58:
/* 04A058 8006EC58 00651823 */  subu  $v1, $v1, $a1
.L8006EC5C:
/* 04A05C 8006EC5C 14C00002 */  bnez  $a2, .L8006EC68
/* 04A060 8006EC60 0066001B */   divu  $zero, $v1, $a2
/* 04A064 8006EC64 0007000D */  break 7
.L8006EC68:
/* 04A068 8006EC68 00002012 */  mflo  $a0
/* 04A06C 8006EC6C 00001810 */  mfhi  $v1
/* 04A070 8006EC70 00000000 */  nop   
/* 04A074 8006EC74 00000000 */  nop   
/* 04A078 8006EC78 008C0018 */  mult  $a0, $t4
/* 04A07C 8006EC7C 3142FFFF */  andi  $v0, $t2, 0xffff
/* 04A080 8006EC80 00031C00 */  sll   $v1, $v1, 0x10
/* 04A084 8006EC84 00621825 */  or    $v1, $v1, $v0
/* 04A088 8006EC88 00002812 */  mflo  $a1
/* 04A08C 8006EC8C 0065102B */  sltu  $v0, $v1, $a1
/* 04A090 8006EC90 5040000B */  beql  $v0, $zero, .L8006ECC0
/* 04A094 8006EC94 00655023 */   subu  $t2, $v1, $a1
/* 04A098 8006EC98 00691821 */  addu  $v1, $v1, $t1
/* 04A09C 8006EC9C 0069102B */  sltu  $v0, $v1, $t1
/* 04A0A0 8006ECA0 14400006 */  bnez  $v0, .L8006ECBC
/* 04A0A4 8006ECA4 2484FFFF */   addiu $a0, $a0, -1
/* 04A0A8 8006ECA8 0065102B */  sltu  $v0, $v1, $a1
/* 04A0AC 8006ECAC 10400004 */  beqz  $v0, .L8006ECC0
/* 04A0B0 8006ECB0 00655023 */   subu  $t2, $v1, $a1
/* 04A0B4 8006ECB4 2484FFFF */  addiu $a0, $a0, -1
/* 04A0B8 8006ECB8 00691821 */  addu  $v1, $v1, $t1
.L8006ECBC:
/* 04A0BC 8006ECBC 00655023 */  subu  $t2, $v1, $a1
.L8006ECC0:
/* 04A0C0 8006ECC0 00071400 */  sll   $v0, $a3, 0x10
/* 04A0C4 8006ECC4 00441025 */  or    $v0, $v0, $a0
/* 04A0C8 8006ECC8 00480019 */  multu $v0, $t0
/* 04A0CC 8006ECCC 00003010 */  mfhi  $a2
/* 04A0D0 8006ECD0 0146102B */  sltu  $v0, $t2, $a2
/* 04A0D4 8006ECD4 00002812 */  mflo  $a1
/* 04A0D8 8006ECD8 54400005 */  bnezl $v0, .L8006ECF0
/* 04A0DC 8006ECDC 00A82023 */   subu  $a0, $a1, $t0
/* 04A0E0 8006ECE0 14CA0007 */  bne   $a2, $t2, .L8006ED00
/* 04A0E4 8006ECE4 0165102B */   sltu  $v0, $t3, $a1
/* 04A0E8 8006ECE8 10400005 */  beqz  $v0, .L8006ED00
/* 04A0EC 8006ECEC 00A82023 */   subu  $a0, $a1, $t0
.L8006ECF0:
/* 04A0F0 8006ECF0 00C91823 */  subu  $v1, $a2, $t1
/* 04A0F4 8006ECF4 00A4102B */  sltu  $v0, $a1, $a0
/* 04A0F8 8006ECF8 00623023 */  subu  $a2, $v1, $v0
/* 04A0FC 8006ECFC 0080282D */  move  $a1, $a0
.L8006ED00:
/* 04A100 8006ED00 1320000A */  beqz  $t9, .L8006ED2C
/* 04A104 8006ED04 01651823 */   subu  $v1, $t3, $a1
/* 04A108 8006ED08 01462023 */  subu  $a0, $t2, $a2
/* 04A10C 8006ED0C 0163102B */  sltu  $v0, $t3, $v1
/* 04A110 8006ED10 00825023 */  subu  $t2, $a0, $v0
/* 04A114 8006ED14 030A1004 */  sllv  $v0, $t2, $t8
/* 04A118 8006ED18 01A31806 */  srlv  $v1, $v1, $t5
/* 04A11C 8006ED1C 00437825 */  or    $t7, $v0, $v1
/* 04A120 8006ED20 01AA7006 */  srlv  $t6, $t2, $t5
func_8006ED24:
/* 04A124 8006ED24 AF2E0000 */  sw    $t6, ($t9)
/* 04A128 8006ED28 AF2F0004 */  sw    $t7, 4($t9)
func_8006ED2C:
.L8006ED2C:
/* 04A12C 8006ED2C 8FA20000 */  lw    $v0, ($sp)
/* 04A130 8006ED30 8FA30004 */  lw    $v1, 4($sp)
/* 04A134 8006ED34 03E00008 */  jr    $ra
/* 04A138 8006ED38 27BD0008 */   addiu $sp, $sp, 8

/* 04A13C 8006ED3C 00000000 */  nop   
func_8006ED40:
/* 04A140 8006ED40 90A20000 */  lbu   $v0, ($a1)
/* 04A144 8006ED44 10400007 */  beqz  $v0, .L8006ED64
/* 04A148 8006ED48 00801821 */   move  $v1, $a0
.L8006ED4C:
/* 04A14C 8006ED4C 90A20000 */  lbu   $v0, ($a1)
/* 04A150 8006ED50 A0620000 */  sb    $v0, ($v1)
/* 04A154 8006ED54 24A50001 */  addiu $a1, $a1, 1
/* 04A158 8006ED58 90A20000 */  lbu   $v0, ($a1)
/* 04A15C 8006ED5C 1440FFFB */  bnez  $v0, .L8006ED4C
/* 04A160 8006ED60 24630001 */   addiu $v1, $v1, 1
.L8006ED64:
/* 04A164 8006ED64 A0600000 */  sb    $zero, ($v1)
/* 04A168 8006ED68 03E00008 */  jr    $ra
/* 04A16C 8006ED6C 00801021 */   move  $v0, $a0

func_8006ED70:
.L8006ED70:
/* 04A170 8006ED70 90860000 */  lbu   $a2, ($a0)
/* 04A174 8006ED74 24840001 */  addiu $a0, $a0, 1
/* 04A178 8006ED78 90A20000 */  lbu   $v0, ($a1)
/* 04A17C 8006ED7C 30C300FF */  andi  $v1, $a2, 0xff
/* 04A180 8006ED80 304200FF */  andi  $v0, $v0, 0xff
/* 04A184 8006ED84 10620003 */  beq   $v1, $v0, .L8006ED94
/* 04A188 8006ED88 24A50001 */   addiu $a1, $a1, 1
/* 04A18C 8006ED8C 0801BB67 */  j     func_8006ED9C
/* 04A190 8006ED90 00621023 */   subu  $v0, $v1, $v0

.L8006ED94:
/* 04A194 8006ED94 14C0FFF6 */  bnez  $a2, .L8006ED70
/* 04A198 8006ED98 00001021 */   move  $v0, $zero
func_8006ED9C:
/* 04A19C 8006ED9C 03E00008 */  jr    $ra
/* 04A1A0 8006EDA0 00000000 */   nop   

/* 04A1A4 8006EDA4 00000000 */  nop   
/* 04A1A8 8006EDA8 00000000 */  nop   
/* 04A1AC 8006EDAC 00000000 */  nop   