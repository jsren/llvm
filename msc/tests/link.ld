SECTIONS
{
    .tinfo : {
        __tinfo_start = .;
        *(.tinfo);
        __tinfo_end = .;
    }
}
INSERT BEFORE .text
