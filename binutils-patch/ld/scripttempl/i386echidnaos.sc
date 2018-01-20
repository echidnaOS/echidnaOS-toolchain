cat <<EOF

ENTRY(_start)

SECTIONS
{
  .text 0x1010000 : { *(.text) }
  .data : { *(.rodata) *(.data) *(.bss) *(COMMON) }
}

EOF
