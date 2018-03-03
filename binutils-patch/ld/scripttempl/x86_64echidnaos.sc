cat <<EOF

ENTRY(_start)

SECTIONS
{
    . = 0x1010000;

    .text ALIGN(4K) :
    {
        *(.text)
    }

    .data ALIGN(4K) :
    {
        *(.data)
        *(.rodata)
    }

    .got ALIGN(4K) :
    {
        _GLOBAL_OFFSET_TABLE_ = .;
        *(.got)
    }

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
    }
}

EOF
