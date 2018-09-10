CALL build clear
tools\timethis build EEx80 all
CALL build clear
tools\timethis build ELx85 all
CALL build clear
tools\timethis build ELx80 all
@pause

del /q .\KeilMisc\Obj\*.*
del /q .\KeilMisc\\Lst\*.*