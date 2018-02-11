cat <<EOF

OUTPUT_FORMAT(binary)
ENTRY(_start)

SECTIONS
{
  .text 0x1010000 : { *(.text) }
  .got : { _GLOBAL_OFFSET_TABLE_ = .; *(.got) }
  .data : { *(.rodata) *(.data) *(.bss) *(COMMON) }
}

EOF
