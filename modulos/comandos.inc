//GENERALES

CMD:help(playerid, params[])
{
    new ayudaList[4024] = "{b6ff86}Server help:\n";
    strcat(ayudaList, "{FFFFFF}- If you have questions, you can use the /doubt command to have an administrator read it..\n");
    strcat(ayudaList, "{FFFFFF}- To see the list of available commands, use /commands.\n");
    strcat(ayudaList, "{FFFFFF}- To see the server rules, use /rules.\n");
    strcat(ayudaList, "{FFFFFF}- If you need additional assistance, please contact an admin via Discord.\n");

    ShowPlayerDialog(playerid, DIALOG_AYUDA, DIALOG_STYLE_MSGBOX, "Server Help", ayudaList, "Close", "");
    return 1;
}

CMD:credits(playerid, params[])
{
    new str[150 + 1];

    format(str, sizeof(str), "%s{AB0707}Host: Cristhian Vera.\n\n{3CFF38}Scripters: Fer Salgado & Cristhian Vera.\n\n{FFB121}Owner(s): Vacant.", str);

    ShowPlayerDialog(playerid, DIALOG_CREDITOS, DIALOG_STYLE_MSGBOX, "{AB671F}Credits", str, "Accept", "Cancel");
    return 1;
}

CMD:report(playerid, params[])
{
    if (!IsPlayerConnected(playerid))
    {
        return 0;
    }

    new target_id, motivo[128];
    if (sscanf(params, "us[128]", target_id, motivo))
    {
        SendClientMessage(playerid, 0xFF0000FF, "USAGE: /report [player ID] [reason]");
        return 1;
    }

    // Verificar si el jugador objetivo existe y está conectado
    if (!IsPlayerConnected(target_id) || target_id == INVALID_PLAYER_ID)
    {
        SendClientMessage(playerid, COLOR_RED, "Player not found or not online.");
        return 1;
    }

    // Verificar si el jugador no se está reportando a sí mismo
    if (target_id == playerid)
    {
        SendClientMessage(playerid, 0xFF0000FF, "You can't report yourself.");
        return 1;
    }

    // Verificar si hay espacio para más reportes
    if (Reporte_Count >= MAX_PLAYERS)
    {
        SendClientMessage(playerid, COLOR_RED, "The reporting system is full. Please try again later.");
        return 1;
    }

    // Almacenar el reporte
    Reportes[Reporte_Count][REPORT_PLAYER_ID] = playerid; // Quién reporta
    Reportes[Reporte_Count][REPORT_TARGET_ID] = target_id; // A quién se reporta
    format(Reportes[Reporte_Count][REPORT_MESSAGE], 128, motivo); // Motivo del reporte
    Reporte_Count++;

    // Notificar al jugador
    SendClientMessage(playerid, 0x00FF00FF, "Your report has been sent to the administrators.");

    // Notificar a los administradores
    new admin_message[256], reporter_name[MAX_PLAYER_NAME], target_name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, reporter_name, sizeof(reporter_name));
    GetPlayerName(target_id, target_name, sizeof(target_name));
    format(admin_message, sizeof(admin_message), "[REPORT] %s (ID: %d) reported to %s (ID: %d): %s", reporter_name, playerid, target_name, target_id, motivo);

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i) && PlayerInfo[i][pNivelAdmin] >= ADMIN_AYUDANTE)
        {
            SendClientMessage(i, 0xFFA500FF, admin_message); // Color naranja para los reportes
        }
    }

    return 1;
}

CMD:fps(playerid, params[])
{
    new fps = GetPlayerFPS(playerid);
    new mensaje[64];
    format(mensaje, sizeof(mensaje), "Your current FPS are: %d", fps);
    SendClientMessage(playerid, COLOR_GREEN, mensaje);
    return 1;

}

CMD:pl(playerid, params[])
{
    new Float:packetloss = NetStats_PacketLossPercent(playerid);
    new mensaje[64];
    format(mensaje, sizeof(mensaje), "Your Packetloss is: %d", packetloss);
    SendClientMessage(playerid, COLOR_GREEN, mensaje);
    return 1;

}

CMD:skin(playerid, params[])
{
    new skinid;
    if (sscanf(params, "d", skinid)) // Si no se proporciona un ID o es inválido
    {
        SendClientMessage(playerid, COLOR_RED, "Usage: /Skin [ID]");
        return 1;
    }

    if (skinid >= 0 && skinid < 312) // Verificar que el ID esté en el rango válido
    {
        SetPlayerSkinAndSave(playerid, skinid); // Aplicar el skin
    }
    else
    {
        SendClientMessage(playerid, COLOR_RED, "Invalid skin ID. Must be between 0 and 311");
    }
    return 1;
}

CMD:skins(playerid, params[])
{
    // Mostrar el menú de selección de skins
    ShowModelSelectionMenu(playerid, skinlist, "Select Skin");
    return 1;
}

CMD:estadisticas(playerid, params[])
{
    if (textdrawVisible[playerid])
    {
        // Ocultar PlayerTextDraws
        PlayerTextDrawHide(playerid, TextDrawScorePorcentaje[playerid]);
        PlayerTextDrawHide(playerid, TextDrawAsesinatosPorcentaje[playerid]);
        PlayerTextDrawHide(playerid, TextDrawMuertesPorcentaje[playerid]);
        PlayerTextDrawHide(playerid, TextDrawScore[playerid]);
        PlayerTextDrawHide(playerid, TextDrawAsesinatos[playerid]);
        PlayerTextDrawHide(playerid, TextDrawMuertes[playerid]);
        SendClientMessage(playerid, COLOR_YELLOW, "Hidden statistics");
    }
    else
    {
        // Mostrar PlayerTextDraws
        PlayerTextDrawShow(playerid, TextDrawScorePorcentaje[playerid]);
        PlayerTextDrawShow(playerid, TextDrawAsesinatosPorcentaje[playerid]);
        PlayerTextDrawShow(playerid, TextDrawMuertesPorcentaje[playerid]);
        PlayerTextDrawShow(playerid, TextDrawScore[playerid]);
        PlayerTextDrawShow(playerid, TextDrawAsesinatos[playerid]);
        PlayerTextDrawShow(playerid, TextDrawMuertes[playerid]);
        SendClientMessage(playerid, COLOR_YELLOW, "Statistics displayed");
    }

    // Alternar estado
    textdrawVisible[playerid] = !textdrawVisible[playerid];
    return 1;
}

//ADMINISTRACIÓN
//Moderador
CMD:mute(playerid, params[])
{
    // Verificar si el jugador tiene el nivel de administración necesario
    if (PlayerInfo[playerid][pNivelAdmin] < ADMIN_MODERADOR) // Solo moderadores o superior pueden usar este comando
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: You do not have permission to use this command.");
        return 1;
    }

    // Obtener los parámetros del comando
    new targetid, tiempo, motivo[128];
    if (sscanf(params, "uis[128]", targetid, tiempo, motivo))
    {
        SendClientMessage(playerid, COLOR_WHITE, "USAGE: /mute [ID] [Time in minutes] [Reason]");
        return 1;
    }

    // Verificar si el jugador objetivo es válido
    if (!IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: The player is not connected.");
        return 1;
    }

    // Verificar si el jugador objetivo tiene un nivel de administración igual o superior
    if (PlayerInfo[targetid][pNivelAdmin] >= PlayerInfo[playerid][pNivelAdmin])
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: You cannot mute an admin of equal or higher rank.");
        return 1;
    }

    // Obtener el nombre del jugador que mutea y del jugador muteado
    new nombreAdmin[MAX_PLAYER_NAME], nombreTarget[MAX_PLAYER_NAME];
    GetPlayerName(playerid, nombreAdmin, MAX_PLAYER_NAME);
    GetPlayerName(targetid, nombreTarget, MAX_PLAYER_NAME);

    // Calcular el tiempo de mute en segundos
    new tiempoMute = tiempo * 60;

    // Guardar los datos del mute en la base de datos
    new query[512];
    format(query, sizeof(query),
           "UPDATE Jugadores SET \
        pSilenciado = 1, \
        pMotivoSilenciado = '%s', \
        pTiempoSilenciado = %d \
        WHERE pNombre = '%s'",
           motivo, tiempoMute, nombreTarget);

    db_query(database, query);

    // Aplicar el mute al jugador
    PlayerInfo[targetid][pSilenciado] = 1;
    format(PlayerInfo[targetid][pMotivoSilenciado], 128, "%s", motivo);
    PlayerInfo[targetid][pTiempoSilenciado] = gettime() + tiempoMute; // Tiempo de expiración del mute

    // Mensaje global al servidor
    new mensajeGlobal[256];
    format(mensajeGlobal, sizeof(mensajeGlobal), "%s has been muted by the administrator %s. Reason: %s. Time: %d minutes.",
           nombreTarget, nombreAdmin, motivo, tiempo);
    SendClientMessageToAll(COLOR_RED, mensajeGlobal);

    return 1;
}

CMD:unmute(playerid, params[])
{
    // Verificar si el jugador tiene el nivel de administración necesario
    if (PlayerInfo[playerid][pNivelAdmin] < ADMIN_MODERADOR) // Solo moderadores o superior pueden usar este comando
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: You do not have permission to use this command.");
        return 1;
    }

    // Obtener el ID del jugador a desmutear
    new targetid;
    if (sscanf(params, "u", targetid))
    {
        SendClientMessage(playerid, COLOR_WHITE, "USAGE: /unmute [ID]");
        return 1;
    }

    // Verificar si el jugador objetivo es válido
    if (!IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: The player is not connected.");
        return 1;
    }

    // Verificar si el jugador objetivo está muteado
    if (PlayerInfo[targetid][pSilenciado] == 0)
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: This player is not muted.");
        return 1;
    }

    // Obtener el nombre del jugador que desmutea y del jugador desmuteado
    new nombreAdmin[MAX_PLAYER_NAME], nombreTarget[MAX_PLAYER_NAME];
    GetPlayerName(playerid, nombreAdmin, MAX_PLAYER_NAME);
    GetPlayerName(targetid, nombreTarget, MAX_PLAYER_NAME);

    // Desmutear al jugador
    PlayerInfo[targetid][pSilenciado] = 0;
    PlayerInfo[targetid][pTiempoSilenciado] = 0;
    format(PlayerInfo[targetid][pMotivoSilenciado], 128, "");

    // Actualizar la base de datos
    new query[128];
    format(query, sizeof(query),
           "UPDATE Jugadores SET \
        pSilenciado = 0, \
        pMotivoSilenciado = '', \
        pTiempoSilenciado = 0 \
        WHERE pNombre = '%s'",
           nombreTarget);

    db_query(database, query);

    // Mensaje global al servidor
    new mensajeGlobal[128];
    format(mensajeGlobal, sizeof(mensajeGlobal), "The player %s has been unmuted by the administrator %s.",
           nombreTarget, nombreAdmin);
    SendClientMessageToAll(COLOR_GREEN, mensajeGlobal);

    return 1;
}

//Administrador

CMD:ban(playerid, params[])
{
    // Verificar si el jugador tiene el nivel de administración necesario
    if (PlayerInfo[playerid][pNivelAdmin] < ADMIN_MODERADOR) // Solo moderadores o superior pueden usar este comando
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: You do not have permission to use this command.");
        return 1;
    }

    // Obtener los parámetros del comando
    new targetid, motivo[128];
    if (sscanf(params, "us[128]", targetid, motivo))
    {
        SendClientMessage(playerid, COLOR_WHITE, "USAGE: /ban [ID] [Reason]");
        return 1;
    }

    // Verificar si el jugador objetivo es válido
    if (!IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: The player is not connected.");
        return 1;
    }

    // Verificar si el jugador objetivo tiene un nivel de administración igual o superior
    if (PlayerInfo[targetid][pNivelAdmin] >= PlayerInfo[playerid][pNivelAdmin])
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: You cannot ban an admin of equal or higher rank.");
        return 1;
    }

    // Obtener el nombre del jugador que banea y del jugador baneado
    new nombreAdmin[MAX_PLAYER_NAME], nombreTarget[MAX_PLAYER_NAME], ipTarget[16];
    GetPlayerName(playerid, nombreAdmin, MAX_PLAYER_NAME);
    GetPlayerName(targetid, nombreTarget, MAX_PLAYER_NAME);
    GetPlayerIp(targetid, ipTarget, sizeof(ipTarget));

    // Obtener la fecha y hora actual para pFechaBan
    new timestamp = gettime();
    new year, month, day, hour, minute, second;
    TimestampToDate(timestamp, year, month, day, hour, minute, second);

    new fechaBan[24];
    format(fechaBan, sizeof(fechaBan), "%04d-%02d-%02d %02d:%02d:%02d",
           year, month, day, hour, minute, second);

    // Guardar los datos del baneo en la base de datos
    new query[512];
    format(query, sizeof(query),
           "UPDATE Jugadores SET \
        pBaneado = 1, \
        pAdminBan = '%s', \
        pMotivoBan = '%s', \
        pFechaBan = '%s' \
        WHERE pNombre = '%s'",
           nombreAdmin, motivo, fechaBan, nombreTarget);

    db_query(database, query);

    // Aplicar el baneo al jugador
    PlayerInfo[targetid][pBaneado] = 1;
    format(PlayerInfo[targetid][pAdminBan], MAX_PLAYER_NAME, "%s", nombreAdmin);
    format(PlayerInfo[targetid][pMotivoBan], 128, "%s", motivo);
    format(PlayerInfo[targetid][pFechaBan], 24, "%s", fechaBan);

    // Kickear al jugador después de un breve retraso
    SetTimerEx("KickPlayerDelayed", 1000, false, "i", targetid);

    // Mensaje global al servidor
    new mensajeGlobal[256];
    format(mensajeGlobal, sizeof(mensajeGlobal), "%s has been banned by the administrator %s. Reason: %s.",
           nombreTarget, nombreAdmin, motivo);
    SendClientMessageToAll(COLOR_RED, mensajeGlobal);

    return 1;
}

CMD:unban(playerid, params[])
{
    // Verificar si el jugador tiene el nivel de administración necesario
    if (PlayerInfo[playerid][pNivelAdmin] < ADMIN_ADMINISTRADOR) // Solo moderadores o superior pueden usar este comando
    {
        SendClientMessage(playerid, COLOR_RED, "ERROR: You do not have permission to use this command.");
        return 1;
    }

    // Obtener la IP del jugador a desbanear
    new ipTarget[16];
    if (sscanf(params, "s[16]", ipTarget))
    {
        SendClientMessage(playerid, COLOR_WHITE, "USAGE: /unban [IP]");
        return 1;
    }

    // Construir la consulta SQL para desbanear al jugador
    new query[256];
    format(query, sizeof(query),
           "UPDATE Jugadores SET \
        pBaneado = 0, \
        pAdminBan = '', \
        pMotivoBan = '', \
        pFechaBan = '' \
        WHERE pIp = '%s'",
           ipTarget);

    db_query(database, query);

    // Mensaje al administrador
    new mensajeAdmin[128];
    format(mensajeAdmin, sizeof(mensajeAdmin), "You have unbanned the IP %s.", ipTarget);
    SendClientMessage(playerid, COLOR_GREEN, mensajeAdmin);

    return 1;
}