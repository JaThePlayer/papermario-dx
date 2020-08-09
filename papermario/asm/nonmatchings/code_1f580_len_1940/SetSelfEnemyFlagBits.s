.set noat # allow manual use of $at
.set noreorder # don't insert nops after branches

.include "include/macro.inc"

glabel SetSelfEnemyFlagBits
/* 020924 80045524 27BDFFE0 */  addiu $sp, $sp, -0x20
/* 020928 80045528 AFBF0018 */  sw    $ra, 0x18($sp)
/* 02092C 8004552C AFB10014 */  sw    $s1, 0x14($sp)
/* 020930 80045530 AFB00010 */  sw    $s0, 0x10($sp)
/* 020934 80045534 8C82000C */  lw    $v0, 0xc($a0)
/* 020938 80045538 8C900148 */  lw    $s0, 0x148($a0)
/* 02093C 8004553C 8C450004 */  lw    $a1, 4($v0)
/* 020940 80045540 0C0B1EAF */  jal   get_variable
/* 020944 80045544 8C510000 */   lw    $s1, ($v0)
/* 020948 80045548 10400004 */  beqz  $v0, .L8004555C
/* 02094C 8004554C 00111827 */   nor   $v1, $zero, $s1
/* 020950 80045550 8E020000 */  lw    $v0, ($s0)
/* 020954 80045554 08011559 */  j     .L80045564
/* 020958 80045558 00511025 */   or    $v0, $v0, $s1

.L8004555C:
/* 02095C 8004555C 8E020000 */  lw    $v0, ($s0)
/* 020960 80045560 00431024 */  and   $v0, $v0, $v1
.L80045564:
/* 020964 80045564 AE020000 */  sw    $v0, ($s0)
/* 020968 80045568 8FBF0018 */  lw    $ra, 0x18($sp)
/* 02096C 8004556C 8FB10014 */  lw    $s1, 0x14($sp)
/* 020970 80045570 8FB00010 */  lw    $s0, 0x10($sp)
/* 020974 80045574 24020002 */  addiu $v0, $zero, 2
/* 020978 80045578 03E00008 */  jr    $ra
/* 02097C 8004557C 27BD0020 */   addiu $sp, $sp, 0x20

/* 020980 80045580 27BDFFE0 */  addiu $sp, $sp, -0x20
/* 020984 80045584 AFB00010 */  sw    $s0, 0x10($sp)
/* 020988 80045588 0080802D */  daddu $s0, $a0, $zero
/* 02098C 8004558C AFBF001C */  sw    $ra, 0x1c($sp)
/* 020990 80045590 AFB20018 */  sw    $s2, 0x18($sp)
/* 020994 80045594 AFB10014 */  sw    $s1, 0x14($sp)
/* 020998 80045598 8E04014C */  lw    $a0, 0x14c($s0)
/* 02099C 8004559C 8E12000C */  lw    $s2, 0xc($s0)
/* 0209A0 800455A0 0C00EABB */  jal   get_npc_unsafe
/* 0209A4 800455A4 8E110148 */   lw    $s1, 0x148($s0)
/* 0209A8 800455A8 0200202D */  daddu $a0, $s0, $zero
/* 0209AC 800455AC 8E450000 */  lw    $a1, ($s2)
/* 0209B0 800455B0 0C0B1EAF */  jal   get_variable
/* 0209B4 800455B4 0040802D */   daddu $s0, $v0, $zero
/* 0209B8 800455B8 A2220007 */  sb    $v0, 7($s1)
/* 0209BC 800455BC C6000038 */  lwc1  $f0, 0x38($s0)
/* 0209C0 800455C0 4600008D */  trunc.w.s $f2, $f0
/* 0209C4 800455C4 44021000 */  mfc1  $v0, $f2
/* 0209C8 800455C8 00000000 */  nop   
/* 0209CC 800455CC A6220010 */  sh    $v0, 0x10($s1)
/* 0209D0 800455D0 C600003C */  lwc1  $f0, 0x3c($s0)
/* 0209D4 800455D4 4600008D */  trunc.w.s $f2, $f0
/* 0209D8 800455D8 44021000 */  mfc1  $v0, $f2
/* 0209DC 800455DC 00000000 */  nop   
/* 0209E0 800455E0 A6220012 */  sh    $v0, 0x12($s1)
/* 0209E4 800455E4 C6000040 */  lwc1  $f0, 0x40($s0)
/* 0209E8 800455E8 4600008D */  trunc.w.s $f2, $f0
/* 0209EC 800455EC 44031000 */  mfc1  $v1, $f2
/* 0209F0 800455F0 00000000 */  nop   
/* 0209F4 800455F4 A6230014 */  sh    $v1, 0x14($s1)
/* 0209F8 800455F8 8FBF001C */  lw    $ra, 0x1c($sp)
/* 0209FC 800455FC 8FB20018 */  lw    $s2, 0x18($sp)
/* 020A00 80045600 8FB10014 */  lw    $s1, 0x14($sp)
/* 020A04 80045604 8FB00010 */  lw    $s0, 0x10($sp)
/* 020A08 80045608 24020002 */  addiu $v0, $zero, 2
/* 020A0C 8004560C 03E00008 */  jr    $ra
/* 020A10 80045610 27BD0020 */   addiu $sp, $sp, 0x20
