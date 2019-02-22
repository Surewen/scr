# Projekt na SCR

### Kompilacja
```
./compile.sh
```

### bank
```
./bank <adres_wlasny>
```

Ma 1 proces który nasłuchuje na żądania i odpowiada na nie.

### atm
```
./atm <adres_wlasny> <adres_banku>
```

Ma 2 procesy - jeden do komunikacji z bankiem i drugi do obsługi klienta (odpowiednio atm_communication i atm_service)

