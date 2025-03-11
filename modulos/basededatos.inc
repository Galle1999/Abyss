// Módulo: basededatos.inc

// Función para crear la base de datos y la tabla de jugadores
stock CrearTablaJugadores()
{
    // Consulta SQL para crear la tabla Jugadores si no existe
    new query[2048];
    format(query, sizeof(query),
        "CREATE TABLE IF NOT EXISTS Jugadores ( \
            pID INTEGER PRIMARY KEY AUTOINCREMENT, \
            pNombre TEXT NOT NULL, \
            pPassword TEXT NOT NULL, \
            pIp TEXT NOT NULL, \
            pPais TEXT(64) NOT NULL, \
            pUltimaConexion TEXT NOT NULL, \
            pRegistrado INTEGER DEFAULT 0, \
            pScore INTEGER DEFAULT 0, \
            pAsesinatos INTEGER DEFAULT 0, \
            pMuertes INTEGER DEFAULT 0, \
            pDuelosGanados INTEGER DEFAULT 0, \
            pDuelosPerdidos INTEGER DEFAULT 0, \
            pDinero INTEGER DEFAULT 0, \
            pSkin INTEGER DEFAULT 0, \
            pClima INTEGER DEFAULT 0, \
            pHora INTEGER DEFAULT 0, \
            pNivelAdmin INTEGER DEFAULT 0, \
            pNivel INTEGER DEFAULT 0, \
            pSilenciado INTEGER DEFAULT 0, \
            pMotivoSilenciado TEXT DEFAULT '', \
            pTiempoSilenciado INTEGER DEFAULT 0, \
            pJail INTEGER DEFAULT 0, \
            pTiempoJail INTEGER DEFAULT 0, \
            pBaneado INTEGER DEFAULT 0, \
            pAdminBan TEXT DEFAULT '', \
            pMotivoBan TEXT DEFAULT '', \
            pFechaBan TEXT DEFAULT '' \
        )"
    );

    // Ejecutar la consulta
    db_query(database, query);
    printf("Tabla Jugadores creada o verificada correctamente.");
    return 1;
}

stock CargarDatosJugador(playerid)
{
    new query[256], nombre[MAX_PLAYER_NAME], ip[16];
    GetPlayerName(playerid, nombre, MAX_PLAYER_NAME);
    GetPlayerIp(playerid, ip, sizeof(ip));

    format(query, sizeof(query), "SELECT * FROM Jugadores WHERE pNombre = '%s'", nombre);
    new DBResult:resultado = db_query(database, query);

    if (db_num_rows(resultado) > 0)
    {
        // Cargar datos del jugador
        PlayerInfo[playerid][pID] = db_get_field_assoc_int(resultado, "pID");
        db_get_field_assoc(resultado, "pNombre", PlayerInfo[playerid][pNombre], MAX_PLAYER_NAME);
        db_get_field_assoc(resultado, "pPassword", PlayerInfo[playerid][pPassword], 129);
        db_get_field_assoc(resultado, "pIp", PlayerInfo[playerid][pIp], 16);
        db_get_field_assoc(resultado, "pPais", PlayerInfo[playerid][pPais], 64);
        db_get_field_assoc(resultado, "pUltimaConexion", PlayerInfo[playerid][pUltimaConexion], 24);
        PlayerInfo[playerid][pRegistrado] = db_get_field_assoc_int(resultado, "pRegistrado");
        PlayerInfo[playerid][pScore] = db_get_field_assoc_int(resultado, "pScore");
        PlayerInfo[playerid][pAsesinatos] = db_get_field_assoc_int(resultado, "pAsesinatos");
        PlayerInfo[playerid][pMuertes] = db_get_field_assoc_int(resultado, "pMuertes");
        PlayerInfo[playerid][pDuelosGanados] = db_get_field_assoc_int(resultado, "pDuelosGanados");
        PlayerInfo[playerid][pDuelosPerdidos] = db_get_field_assoc_int(resultado, "pDuelosPerdidos");
        PlayerInfo[playerid][pDinero] = db_get_field_assoc_int(resultado, "pDinero");
        PlayerInfo[playerid][pSkin] = db_get_field_assoc_int(resultado, "pSkin");
        PlayerInfo[playerid][pClima] = db_get_field_assoc_int(resultado, "pClima");
        PlayerInfo[playerid][pHora] = db_get_field_assoc_int(resultado, "pHora");
        PlayerInfo[playerid][pNivelAdmin] = db_get_field_assoc_int(resultado, "pNivelAdmin");
        PlayerInfo[playerid][pNivel] = db_get_field_assoc_int(resultado, "pNivel");
        PlayerInfo[playerid][pSilenciado] = db_get_field_assoc_int(resultado, "pSilenciado");
        db_get_field_assoc(resultado, "pMotivoSilenciado", PlayerInfo[playerid][pMotivoSilenciado], 128);
        PlayerInfo[playerid][pTiempoSilenciado] = db_get_field_assoc_int(resultado, "pTiempoSilenciado");
        PlayerInfo[playerid][pJail] = db_get_field_assoc_int(resultado, "pJail");
        PlayerInfo[playerid][pTiempoJail] = db_get_field_assoc_int(resultado, "pTiempoJail"); // Para el sistema de cárcel (a futuro)
        PlayerInfo[playerid][pBaneado] = db_get_field_assoc_int(resultado, "pBaneado");
        db_get_field_assoc(resultado, "pAdminBan", PlayerInfo[playerid][pAdminBan], MAX_PLAYER_NAME);
        db_get_field_assoc(resultado, "pMotivoBan", PlayerInfo[playerid][pMotivoBan], 128);
        db_get_field_assoc(resultado, "pFechaBan", PlayerInfo[playerid][pFechaBan], 20);
        PlayerInfo[playerid][pRachas] = db_get_field_assoc_int(resultado, "pRachas");
        db_get_field_assoc(resultado, "pTargetName", PlayerInfo[playerid][pTargetName], MAX_PLAYER_NAME);

        // Verificar si el jugador está baneado
        if (PlayerInfo[playerid][pBaneado] == 1)
        {
            // Crear el mensaje del diálogo
            new mensajeBaneado[512];
            format(mensajeBaneado, sizeof(mensajeBaneado),
                "You have been banned.\n\nReason: %s\nDate: %s\nAdministrator: %s\n\nIf you think this was a mistake, please appeal on our Discord.\n\nhttps://discord.com/invite/Pqcuhhav3T.",
                PlayerInfo[playerid][pMotivoBan],
                PlayerInfo[playerid][pFechaBan],
                PlayerInfo[playerid][pAdminBan]
            );
            SendClientMessage(playerid, COLOR_RED, "You have been banned from the server.\n{FFFF00}Remember to take a screenshot of the ban when making reports or requests to be unbanned.");
            GameTextForPlayer(playerid, "~r~Banned", 500, 5);

            // Mostrar el diálogo de tipo msgbox
            ShowPlayerDialog(playerid, DIALOG_BAN, DIALOG_STYLE_MSGBOX, "Banned", mensajeBaneado, "Close", "");

            // Kickear al jugador después de un breve retraso
            SetTimerEx("KickPlayerDelayed", 3000, false, "i", playerid); // 5 segundos de retraso
        }

        if (PlayerInfo[playerid][pSilenciado] == 1 && PlayerInfo[playerid][pTiempoSilenciado] > gettime())
        {
            new tiempoRestante = PlayerInfo[playerid][pTiempoSilenciado] - gettime();
            new minutosRestantes = tiempoRestante / 60;

            // Mensaje al jugador muteado
            new mensajeMuteado[256];
            format(mensajeMuteado, sizeof(mensajeMuteado), "You are muted. Reason: %s. Time: %d minutes.",
                PlayerInfo[playerid][pMotivoSilenciado], minutosRestantes);
            SendClientMessage(playerid, COLOR_RED, mensajeMuteado);
        }
        else if (PlayerInfo[playerid][pSilenciado] == 1 && PlayerInfo[playerid][pTiempoSilenciado] <= gettime())
        {
            // Si el tiempo de mute ha expirado, desmuteamos al jugador
            PlayerInfo[playerid][pSilenciado] = 0;
            PlayerInfo[playerid][pTiempoSilenciado] = 0;
            format(PlayerInfo[playerid][pMotivoSilenciado], 128, "");

            // Actualizar la base de datos
            new queryActualizar[128];
            format(queryActualizar, sizeof(queryActualizar),
                "UPDATE Jugadores SET \
                pSilenciado = 0, \
                pMotivoSilenciado = '', \
                pTiempoSilenciado = 0 \
                WHERE pNombre = '%s'",
                nombre);

            db_query(database, queryActualizar);

            // Mensaje al jugador
            SendClientMessage(playerid, COLOR_GREEN, "Your mute has expired. You may now speak again.");
        }

        // Verificar si la IP coincide para autologin
        if (strcmp(PlayerInfo[playerid][pIp], ip, false) == 0)
        {
            SendClientMessage(playerid, COLOR_GREEN, "Autologin.");
            // Enviar mensaje de bienvenida
            SetTimerEx("EnviarMensajeBienvenida", 1000, false, "i", playerid); // 1 segundo de retraso
        }
        else
        {
            ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Enter your password:", "Accept", "Cancel");
        }

        printf("Datos del jugador %s cargados correctamente.", nombre);
    }
    else
    {
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", "Please enter a password to register:", "Accept", "Cancel");
    }

    db_free_result(resultado);
    return 1;
}

stock GuardarDatosJugador(playerid)
{
    new query[2048], nombre[MAX_PLAYER_NAME];
    GetPlayerName(playerid, nombre, MAX_PLAYER_NAME);

    // Obtener la fecha y hora actual para pUltimaConexion
    new timestamp = gettime();
    new year, month, day, hour, minute, second;
    TimestampToDate(timestamp, year, month, day, hour, minute, second);

    new ultimaConexion[24];
    format(ultimaConexion, sizeof(ultimaConexion), "%04d-%02d-%02d %02d:%02d:%02d",
        year, month, day, hour, minute, second);

    // Actualizar pUltimaConexion en PlayerInfo
    format(PlayerInfo[playerid][pUltimaConexion], 24, "%s", ultimaConexion);

    // Construir la consulta SQL
    format(query, sizeof(query),
        "UPDATE Jugadores SET \
        pPassword = '%s', \
        pIp = '%s', \
        pPais = '%s', \
        pUltimaConexion = '%s', \
        pRegistrado = %d, \
        pScore = %d, \
        pAsesinatos = %d, \
        pMuertes = %d, \
        pDuelosGanados = %d, \
        pDuelosPerdidos = %d, \
        pDinero = %d, \
        pSkin = %d, \
        pClima = %d, \
        pHora = %d, \
        pNivelAdmin = %d, \
        pNivel = %d, \
        pSilenciado = %d, \
        pMotivoSilenciado = '%s', \
        pTiempoSilenciado = %d, \
        pJail = %d, \
        pTiempoJail = %d, \
        pBaneado = %d, \
        pAdminBan = '%s', \
        pMotivoBan = '%s', \
        pFechaBan = '%s', \
        pRachas = %d, \
        pTargetName = '%s' \
        WHERE pNombre = '%s'",
        PlayerInfo[playerid][pPassword],
        PlayerInfo[playerid][pIp],
        PlayerInfo[playerid][pPais],
        PlayerInfo[playerid][pUltimaConexion],
        PlayerInfo[playerid][pRegistrado],
        PlayerInfo[playerid][pScore],
        PlayerInfo[playerid][pAsesinatos],
        PlayerInfo[playerid][pMuertes],
        PlayerInfo[playerid][pDuelosGanados],
        PlayerInfo[playerid][pDuelosPerdidos],
        PlayerInfo[playerid][pDinero],
        PlayerInfo[playerid][pSkin],
        PlayerInfo[playerid][pClima],
        PlayerInfo[playerid][pHora],
        PlayerInfo[playerid][pNivelAdmin],
        PlayerInfo[playerid][pNivel],
        PlayerInfo[playerid][pSilenciado],
        PlayerInfo[playerid][pMotivoSilenciado],
        PlayerInfo[playerid][pTiempoSilenciado],
        PlayerInfo[playerid][pJail],
        PlayerInfo[playerid][pTiempoJail],
        PlayerInfo[playerid][pBaneado],
        PlayerInfo[playerid][pAdminBan],
        PlayerInfo[playerid][pMotivoBan],
        PlayerInfo[playerid][pFechaBan],
        PlayerInfo[playerid][pRachas],
        PlayerInfo[playerid][pTargetName],
        nombre
    );
    // Ejecutar la consulta
    db_query(database, query);
    return 1;
}

forward GuardarTodosLosJugadores();
public GuardarTodosLosJugadores()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i))
        {
            GuardarDatosJugador(i);
        }
    }
    printf("Datos de todos los jugadores guardados.");
    return 1;
}

forward OnPasswordHashed(playerid);
public OnPasswordHashed(playerid)
{
    new hash[BCRYPT_HASH_LENGTH], pais[64], ip[16], nombre[MAX_PLAYER_NAME];
    bcrypt_get_hash(hash); // Obtener el hash de la contraseña

    // Obtener el país y la IP del jugador
    GetPlayerCountry(playerid, pais, sizeof(pais));
    GetPlayerIp(playerid, ip, sizeof(ip));
    GetPlayerName(playerid, nombre, MAX_PLAYER_NAME);

    // Obtener la fecha y hora actual
    new timestamp = gettime();
    new year, month, day, hour, minute, second;
    TimestampToDate(timestamp, year, month, day, hour, minute, second);

    // Formatear la fecha y hora como una cadena
    new ultimaConexion[24];
    format(ultimaConexion, sizeof(ultimaConexion), "%04d-%02d-%02d %02d:%02d:%02d",
        year, month, day, hour, minute, second);

    // Consulta SQL para insertar todos los campos
    new query[2048];
    format(query, sizeof(query),
        "INSERT INTO Jugadores (\
            pNombre, pPassword, pIp, pPais, pUltimaConexion, pRegistrado, \
            pScore, pAsesinatos, pMuertes, pDuelosGanados, pDuelosPerdidos, \
            pDinero, pSkin, pClima, pHora, pNivelAdmin, pNivel, pSilenciado, \
            pMotivoSilenciado, pTiempoSilenciado, pJail, pTiempoJail, \
            pBaneado, pAdminBan, pMotivoBan, pFechaBan\
        ) VALUES (\
            '%s', '%s', '%s', '%s', '%s', 1, \
            0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0,\
            '', 0, 0, 0, \
            0, '', '', ''\
        )",
        nombre, hash, ip, pais, ultimaConexion
    );

    // Ejecutar la consulta
    db_query(database, query);

    // Mensaje de registro exitoso (opcional)
    SendClientMessage(playerid, COLOR_GREEN, "Registration successful! Welcome!");
    return 1;
}

forward OnPasswordVerified(playerid, bool:success);
public OnPasswordVerified(playerid, bool:success)
{
    if (!success)
    {
        SendClientMessage(playerid, COLOR_RED, "Incorrect password. Try again.");
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Enter your password:", "Accept", "Cancel");
        return 1;
    }

    // Login exitoso
    SendClientMessage(playerid, COLOR_GREEN, "Login successful. Welcome back.");

    // Aquí puedes cargar más datos del jugador o realizar otras acciones necesarias
    return 1;
}