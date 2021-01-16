.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches

glabel func_802194A0_43AF30
/* 43AF30 802194A0 27BDFFA8 */  addiu     $sp, $sp, -0x58
/* 43AF34 802194A4 AFB40020 */  sw        $s4, 0x20($sp)
/* 43AF38 802194A8 0080A02D */  daddu     $s4, $a0, $zero
/* 43AF3C 802194AC AFBF0024 */  sw        $ra, 0x24($sp)
/* 43AF40 802194B0 AFB3001C */  sw        $s3, 0x1c($sp)
/* 43AF44 802194B4 AFB20018 */  sw        $s2, 0x18($sp)
/* 43AF48 802194B8 AFB10014 */  sw        $s1, 0x14($sp)
/* 43AF4C 802194BC AFB00010 */  sw        $s0, 0x10($sp)
/* 43AF50 802194C0 F7BE0050 */  sdc1      $f30, 0x50($sp)
/* 43AF54 802194C4 F7BC0048 */  sdc1      $f28, 0x48($sp)
/* 43AF58 802194C8 F7BA0040 */  sdc1      $f26, 0x40($sp)
/* 43AF5C 802194CC F7B80038 */  sdc1      $f24, 0x38($sp)
/* 43AF60 802194D0 F7B60030 */  sdc1      $f22, 0x30($sp)
/* 43AF64 802194D4 F7B40028 */  sdc1      $f20, 0x28($sp)
/* 43AF68 802194D8 8E90000C */  lw        $s0, 0xc($s4)
/* 43AF6C 802194DC 8E050000 */  lw        $a1, ($s0)
/* 43AF70 802194E0 0C0B1EAF */  jal       get_variable
/* 43AF74 802194E4 26100004 */   addiu    $s0, $s0, 4
/* 43AF78 802194E8 44820000 */  mtc1      $v0, $f0
/* 43AF7C 802194EC 00000000 */  nop
/* 43AF80 802194F0 46800021 */  cvt.d.w   $f0, $f0
/* 43AF84 802194F4 8E110000 */  lw        $s1, ($s0)
/* 43AF88 802194F8 26100004 */  addiu     $s0, $s0, 4
/* 43AF8C 802194FC 3C014024 */  lui       $at, 0x4024
/* 43AF90 80219500 44811800 */  mtc1      $at, $f3
/* 43AF94 80219504 44801000 */  mtc1      $zero, $f2
/* 43AF98 80219508 3C0140C9 */  lui       $at, 0x40c9
/* 43AF9C 8021950C 34210FD0 */  ori       $at, $at, 0xfd0
/* 43AFA0 80219510 4481A000 */  mtc1      $at, $f20
/* 43AFA4 80219514 8E130000 */  lw        $s3, ($s0)
/* 43AFA8 80219518 46220003 */  div.d     $f0, $f0, $f2
/* 43AFAC 8021951C 462007A0 */  cvt.s.d   $f30, $f0
/* 43AFB0 80219520 4614F502 */  mul.s     $f20, $f30, $f20
/* 43AFB4 80219524 00000000 */  nop
/* 43AFB8 80219528 26100004 */  addiu     $s0, $s0, 4
/* 43AFBC 8021952C 8E120000 */  lw        $s2, ($s0)
/* 43AFC0 80219530 3C0143B4 */  lui       $at, 0x43b4
/* 43AFC4 80219534 44810000 */  mtc1      $at, $f0
/* 43AFC8 80219538 8E100004 */  lw        $s0, 4($s0)
/* 43AFCC 8021953C 4600A503 */  div.s     $f20, $f20, $f0
/* 43AFD0 80219540 0C00A85B */  jal       sin_rad
/* 43AFD4 80219544 4600A306 */   mov.s    $f12, $f20
/* 43AFD8 80219548 3C014100 */  lui       $at, 0x4100
/* 43AFDC 8021954C 44816000 */  mtc1      $at, $f12
/* 43AFE0 80219550 00000000 */  nop
/* 43AFE4 80219554 460CA302 */  mul.s     $f12, $f20, $f12
/* 43AFE8 80219558 00000000 */  nop
/* 43AFEC 8021955C 3C014316 */  lui       $at, 0x4316
/* 43AFF0 80219560 4481E000 */  mtc1      $at, $f28
/* 43AFF4 80219564 00000000 */  nop
/* 43AFF8 80219568 461C0582 */  mul.s     $f22, $f0, $f28
/* 43AFFC 8021956C 00000000 */  nop
/* 43B000 80219570 4480D000 */  mtc1      $zero, $f26
/* 43B004 80219574 0C00A85B */  jal       sin_rad
/* 43B008 80219578 461AB580 */   add.s    $f22, $f22, $f26
/* 43B00C 8021957C 3C014120 */  lui       $at, 0x4120
/* 43B010 80219580 44811000 */  mtc1      $at, $f2
/* 43B014 80219584 00000000 */  nop
/* 43B018 80219588 46020602 */  mul.s     $f24, $f0, $f2
/* 43B01C 8021958C 00000000 */  nop
/* 43B020 80219590 4600A306 */  mov.s     $f12, $f20
/* 43B024 80219594 0C00A874 */  jal       cos_rad
/* 43B028 80219598 461AC600 */   add.s    $f24, $f24, $f26
/* 43B02C 8021959C 0280202D */  daddu     $a0, $s4, $zero
/* 43B030 802195A0 461C0502 */  mul.s     $f20, $f0, $f28
/* 43B034 802195A4 00000000 */  nop
/* 43B038 802195A8 0220282D */  daddu     $a1, $s1, $zero
/* 43B03C 802195AC 4406B000 */  mfc1      $a2, $f22
/* 43B040 802195B0 3C0141A0 */  lui       $at, 0x41a0
/* 43B044 802195B4 44810000 */  mtc1      $at, $f0
/* 43B048 802195B8 3C01C248 */  lui       $at, 0xc248
/* 43B04C 802195BC 44811000 */  mtc1      $at, $f2
/* 43B050 802195C0 4600A503 */  div.s     $f20, $f20, $f0
/* 43B054 802195C4 0C0B2190 */  jal       set_float_variable
/* 43B058 802195C8 4602A500 */   add.s    $f20, $f20, $f2
/* 43B05C 802195CC 0280202D */  daddu     $a0, $s4, $zero
/* 43B060 802195D0 4406C000 */  mfc1      $a2, $f24
/* 43B064 802195D4 0C0B2190 */  jal       set_float_variable
/* 43B068 802195D8 0260282D */   daddu    $a1, $s3, $zero
/* 43B06C 802195DC 0280202D */  daddu     $a0, $s4, $zero
/* 43B070 802195E0 4406A000 */  mfc1      $a2, $f20
/* 43B074 802195E4 0C0B2190 */  jal       set_float_variable
/* 43B078 802195E8 0240282D */   daddu    $a1, $s2, $zero
/* 43B07C 802195EC 3C0142B4 */  lui       $at, 0x42b4
/* 43B080 802195F0 44810000 */  mtc1      $at, $f0
/* 43B084 802195F4 00000000 */  nop
/* 43B088 802195F8 461E003C */  c.lt.s    $f0, $f30
/* 43B08C 802195FC 00000000 */  nop
/* 43B090 80219600 4500000B */  bc1f      .L80219630
/* 43B094 80219604 0000302D */   daddu    $a2, $zero, $zero
/* 43B098 80219608 3C014387 */  lui       $at, 0x4387
/* 43B09C 8021960C 44810000 */  mtc1      $at, $f0
/* 43B0A0 80219610 00000000 */  nop
/* 43B0A4 80219614 4600F03C */  c.lt.s    $f30, $f0
/* 43B0A8 80219618 00000000 */  nop
/* 43B0AC 8021961C 45030001 */  bc1tl     .L80219624
/* 43B0B0 80219620 240600B4 */   addiu    $a2, $zero, 0xb4
.L80219624:
/* 43B0B4 80219624 3C0142B4 */  lui       $at, 0x42b4
/* 43B0B8 80219628 44810000 */  mtc1      $at, $f0
/* 43B0BC 8021962C 00000000 */  nop
.L80219630:
/* 43B0C0 80219630 461E0001 */  sub.s     $f0, $f0, $f30
/* 43B0C4 80219634 46000005 */  abs.s     $f0, $f0
/* 43B0C8 80219638 4600010D */  trunc.w.s $f4, $f0
/* 43B0CC 8021963C 44022000 */  mfc1      $v0, $f4
/* 43B0D0 80219640 00000000 */  nop
/* 43B0D4 80219644 2842002D */  slti      $v0, $v0, 0x2d
/* 43B0D8 80219648 10400008 */  beqz      $v0, .L8021966C
/* 43B0DC 8021964C 00000000 */   nop
/* 43B0E0 80219650 3C014234 */  lui       $at, 0x4234
/* 43B0E4 80219654 44810000 */  mtc1      $at, $f0
/* 43B0E8 80219658 00000000 */  nop
/* 43B0EC 8021965C 4600F001 */  sub.s     $f0, $f30, $f0
/* 43B0F0 80219660 46000000 */  add.s     $f0, $f0, $f0
/* 43B0F4 80219664 4600010D */  trunc.w.s $f4, $f0
/* 43B0F8 80219668 44062000 */  mfc1      $a2, $f4
.L8021966C:
/* 43B0FC 8021966C 3C014387 */  lui       $at, 0x4387
/* 43B100 80219670 44810000 */  mtc1      $at, $f0
/* 43B104 80219674 00000000 */  nop
/* 43B108 80219678 461E0001 */  sub.s     $f0, $f0, $f30
/* 43B10C 8021967C 46000005 */  abs.s     $f0, $f0
/* 43B110 80219680 4600010D */  trunc.w.s $f4, $f0
/* 43B114 80219684 44022000 */  mfc1      $v0, $f4
/* 43B118 80219688 00000000 */  nop
/* 43B11C 8021968C 2842002D */  slti      $v0, $v0, 0x2d
/* 43B120 80219690 1040000C */  beqz      $v0, .L802196C4
/* 43B124 80219694 0280202D */   daddu    $a0, $s4, $zero
/* 43B128 80219698 3C014361 */  lui       $at, 0x4361
/* 43B12C 8021969C 44810000 */  mtc1      $at, $f0
/* 43B130 802196A0 00000000 */  nop
/* 43B134 802196A4 4600F001 */  sub.s     $f0, $f30, $f0
/* 43B138 802196A8 46000000 */  add.s     $f0, $f0, $f0
/* 43B13C 802196AC 3C014334 */  lui       $at, 0x4334
/* 43B140 802196B0 44811000 */  mtc1      $at, $f2
/* 43B144 802196B4 00000000 */  nop
/* 43B148 802196B8 46020000 */  add.s     $f0, $f0, $f2
/* 43B14C 802196BC 4600010D */  trunc.w.s $f4, $f0
/* 43B150 802196C0 44062000 */  mfc1      $a2, $f4
.L802196C4:
/* 43B154 802196C4 0C0B2026 */  jal       set_variable
/* 43B158 802196C8 0200282D */   daddu    $a1, $s0, $zero
/* 43B15C 802196CC 8FBF0024 */  lw        $ra, 0x24($sp)
/* 43B160 802196D0 8FB40020 */  lw        $s4, 0x20($sp)
/* 43B164 802196D4 8FB3001C */  lw        $s3, 0x1c($sp)
/* 43B168 802196D8 8FB20018 */  lw        $s2, 0x18($sp)
/* 43B16C 802196DC 8FB10014 */  lw        $s1, 0x14($sp)
/* 43B170 802196E0 8FB00010 */  lw        $s0, 0x10($sp)
/* 43B174 802196E4 D7BE0050 */  ldc1      $f30, 0x50($sp)
/* 43B178 802196E8 D7BC0048 */  ldc1      $f28, 0x48($sp)
/* 43B17C 802196EC D7BA0040 */  ldc1      $f26, 0x40($sp)
/* 43B180 802196F0 D7B80038 */  ldc1      $f24, 0x38($sp)
/* 43B184 802196F4 D7B60030 */  ldc1      $f22, 0x30($sp)
/* 43B188 802196F8 D7B40028 */  ldc1      $f20, 0x28($sp)
/* 43B18C 802196FC 24020002 */  addiu     $v0, $zero, 2
/* 43B190 80219700 03E00008 */  jr        $ra
/* 43B194 80219704 27BD0058 */   addiu    $sp, $sp, 0x58