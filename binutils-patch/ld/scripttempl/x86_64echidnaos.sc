cat <<EOF

OUTPUT_FORMAT(binary)
ENTRY(_start)

SECTIONS
{
    . = 0x1010000;

    .text ALIGN(4K) :
    {
        *(.text)
    }

    .got ALIGN(4K) :
    {
        _GLOBAL_OFFSET_TABLE_ = .;
        *(.got)
    }

    .data ALIGN(4K) :
    {
        *(.rodata)
        *(.data)
        *(.bss)
        *(COMMON)
    }
}

EOF
