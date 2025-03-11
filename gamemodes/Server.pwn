//Librerias
#define SAMP_COMPAT
#include <open.mp>
#include <Pawn.RakNet>
#include <weapon-config>
#include <sscanf2>
#include <discord-connector>
#include <geolite>
#include <strlib>
#include <zcmd>
#include <samp_bcrypt>
#include <mSelection>
#include <BCMD1>

//Modulos
#include "../modulos/variables.inc"
#include "../modulos/funciones.inc"
#include "../modulos/comandos.inc"
#include "../modulos/textdraws.inc"
#include "../modulos/basededatos.inc"

//𝗡𝗼𝘄 𝗽𝗹𝗮𝘆𝗶𝗻𝗴:
//"Michael Jackson - Billie Jean"
//01:57 ───────●─── 02:55
//ㅤ◁ㅤ ❚❚ ㅤ▷ ㅤㅤ↻ ♡

main()
{
    printf(" ");
    printf("  *********************************");
    printf("  *                               *");
    printf("  *     Abyss Deathmatch 2025     *");
    printf("  *           English             *");
    printf("  *                               *");
    printf("  *********************************");
    printf("  *                               *");
    printf("  *      Credits:                 *");
    printf("  *      - .Galle. (Costa Rica)   *");
    printf("  *        Scripter #1            *");
    printf("  *      - JVera (Paraguay)       *");
    printf("  *        Scripter #2 & Hoster   *");
    printf("  *                               *");
    printf("  *********************************");
    printf(" ");
}

public OnGameModeInit()
{
    SetGameModeText("Abyss DM 1.0");
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
    ShowNameTags(true);
    UsePlayerPedAnims();
    DisableInteriorEnterExits();
    SetVehiclePassengerDamage(true);
    SetDisableSyncBugs(true);
    EnableStuntBonusForAll(false);
    // Añadir clases para los jugadores
    const NUM_CLASES = 311;
    for (new i = 0; i < NUM_CLASES; i++)
    {
        AddPlayerClass(
            i + 1,
            369.2121, 173.7104, 1008.3893, // Coordenadas iniciales
            88.9353,           // Ángulo inicial
            WEAPON_FIST, 100,
            WEAPON_FIST, 100,
            WEAPON_FIST, 100
        );
    }

    // Actualizar estado del bot en Discord
    ActualizarEstadoDelBot();

    // Registrar comando Discord
    DCC_CreateCommand("players", "Players connected to the server", "OnDiscordJugadoresCommand");
    DCC_CreateCommand("ip", "Server IP", "OnDiscordIPCommand");
    DCC_CreateCommand("stats", "Player statistics", "OnDiscordStatsCommand");

    // Abrir la base de datos
    database = db_open("usuarios.db");

    // Crear la tabla Jugadores si no existe
    CrearTablaJugadores();

    printf("Gamemode iniciado y comandos de Discord configurados.");
    //Guardar datos constantemente
    SetTimer("GuardarTodosLosJugadores", 300000, true);
    //Fecha
    SetTimer("ActualizarFechaHoraTextDraw", 1000, true);
    //Skins
    skinlist = LoadModelSelectionMenu("skins.txt");
    return 1;
}

public OnGameModeExit()
{
    // Recorrer todos los jugadores conectados
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i)) // Verificar si el jugador está conectado
        {
            GuardarDatosJugador(i); // Guardar los datos del jugador
        }
    }

    // Cerrar la base de datos
    db_close(database);
    return 1;
}

public OnPlayerConnect(playerid)
{
    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));

    // Obtener datos sensibles: país, ciudad e IP (solo para administradores)
    new country[64], city[64], ip[16];
    GetPlayerCountry(playerid, country, sizeof(country));
    GetPlayerCity(playerid, city, sizeof(city));
    GetPlayerIp(playerid, ip, sizeof(ip));

    // Crear un Embed para el canal de administradores (moderator-only)
    new DCC_Embed:adminEmbed = DCC_CreateEmbed(
        "Player connected :desktop: :white_check_mark:", // Título del Embed
        "",                                              // Descripción opcional
        "",                                              // URL opcional
        "",                                              // Timestamp opcional
        0x3498DB,                                       // Color del Embed (azul)
        "Advanced information",                          // Texto del pie de página
        "https://i.imgur.com/s5mjJsW.jpeg",              // URL del icono en el pie de página
        "",                                              // Miniatura
        ""                                               // Imagen principal
    );

    // Añadir detalles sensibles al Embed de administradores
    new mensaje[128];
    format(mensaje, sizeof(mensaje), "**%s** (ID: %d)", pname, playerid);
    DCC_AddEmbedField(adminEmbed, "Player", mensaje, false);
    format(mensaje, sizeof(mensaje), "**Country:** %s\n**City:** %s\n**IP:** %s", country, city, ip);
    DCC_AddEmbedField(adminEmbed, "Details", mensaje, false);

    // Enviar el Embed al canal de administradores
    new DCC_Channel:adminChannel = DCC_FindChannelByName("moderator-only");
    if (adminChannel != DCC_INVALID_CHANNEL)
    {
        DCC_SendChannelEmbedMessage(adminChannel, adminEmbed, "");
    }

    // Liberar memoria del Embed
    DCC_DeleteEmbed(adminEmbed);

    // Actualizar el estado del bot
    ActualizarEstadoDelBot();

    // Netstats Textdraw
    NetstatsTextdraw(playerid);
    logo(playerid);
    fechatextdraw(playerid);
    CreatePlayerStatsTextDraws(playerid);
    TextdrawKills(playerid);

    // Inicializar todos los campos de PlayerInfo
    PlayerInfo[playerid][pNombre][0] = EOS;
    PlayerInfo[playerid][pPassword][0] = EOS;
    PlayerInfo[playerid][pIp][0] = EOS;
    PlayerInfo[playerid][pPais][0] = EOS;
    PlayerInfo[playerid][pUltimaConexion][0] = EOS;
    PlayerInfo[playerid][pRegistrado] = 0;
    PlayerInfo[playerid][pScore] = 0;
    PlayerInfo[playerid][pAsesinatos] = 0;
    PlayerInfo[playerid][pMuertes] = 0;
    PlayerInfo[playerid][pDuelosGanados] = 0;
    PlayerInfo[playerid][pDuelosPerdidos] = 0;
    PlayerInfo[playerid][pDinero] = 0;
    PlayerInfo[playerid][pSkin] = 0;
    PlayerInfo[playerid][pClima] = 0;
    PlayerInfo[playerid][pHora] = 0;
    PlayerInfo[playerid][pNivelAdmin] = 0;
    PlayerInfo[playerid][pSilenciado] = 0;
    PlayerInfo[playerid][pMotivoSilenciado][0] = EOS;
    PlayerInfo[playerid][pTiempoSilenciado] = 0;
    PlayerInfo[playerid][pJail] = 0;
    PlayerInfo[playerid][pTiempoJail] = 0;
    PlayerInfo[playerid][pBaneado] = 0;
    PlayerInfo[playerid][pAdminBan][0] = EOS;
    PlayerInfo[playerid][pMotivoBan][0] = EOS;
    PlayerInfo[playerid][pFechaBan][0] = EOS;

    // Cargar datos del jugador desde la base de datos
    CargarDatosJugador(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));

    // Obtener datos sensibles: país, ciudad e IP (solo para administradores)
    new country[64], city[64], ip[16];
    GetPlayerCountry(playerid, country, sizeof(country));
    GetPlayerCity(playerid, city, sizeof(city));
    GetPlayerIp(playerid, ip, sizeof(ip));

    // Crear un Embed para el canal de administradores (moderator-only)
    new DCC_Embed:adminEmbed = DCC_CreateEmbed(
        "Player disconnected :desktop: :x:", // Título del Embed
        "",                                              // Descripción opcional
        "",                                              // URL opcional
        "",                                              // Timestamp opcional
        0xE74C3C,                                       // Color del Embed (rojo)
        "Advanced information",                          // Texto del pie de página
        "https://i.imgur.com/s5mjJsW.jpeg",              // URL del icono en el pie de página
        "",                                              // Miniatura
        ""                                               // Imagen principal
    );

    // Añadir detalles sensibles al Embed de administradores
    new mensaje[128];
    format(mensaje, sizeof(mensaje), "**%s** (ID: %d)", pname, playerid);
    DCC_AddEmbedField(adminEmbed, "Player", mensaje, false);
    format(mensaje, sizeof(mensaje), "**Country:** %s\n**City:** %s\n**IP:** %s", country, city, ip);
    DCC_AddEmbedField(adminEmbed, "Details", mensaje, false);

    // Enviar el Embed al canal de administradores
    new DCC_Channel:adminChannel = DCC_FindChannelByName("moderator-only");
    if (adminChannel != DCC_INVALID_CHANNEL)
    {
        DCC_SendChannelEmbedMessage(adminChannel, adminEmbed, "");
    }

    // Liberar memoria del Embed
    DCC_DeleteEmbed(adminEmbed);

    // Actualizar estado del bot
    ActualizarEstadoDelBot();

    // Guardar los datos del jugador antes de que se desconecte
    GuardarDatosJugador(playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    SetPlayerInterior(playerid, 3);
    SetPlayerPos(playerid, 349.0453, 193.2271, 1014.1797);
    SetPlayerFacingAngle(playerid, 286.25);
    SetPlayerCameraPos(playerid, 352.9164, 194.5702, 1014.1875);
    SetPlayerCameraLookAt(playerid, 349.0453, 193.2271, 1014.1797);
    ApplyAnimation(playerid, "benchpress", "gym_bp_celebrate", 4.1, true, false, false, false, 0, SYNC_NONE);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerInterior(playerid, 3);
    return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if (killerid != INVALID_PLAYER_ID)
    {
        // Incrementar la racha de asesinatos del jugador
        PlayerInfo[killerid][pRachas]++;

        // Incrementar el puntaje del jugador
        SetPlayerScore(killerid, GetPlayerScore(killerid) + 1);

        // Otorgar dinero al jugador
        GivePlayerMoney(killerid, 1000);

        // Incrementar el score en PlayerInfo
        PlayerInfo[killerid][pScore]++;

        // Incrementar las estadísticas de asesinatos del jugador
        PlayerInfo[killerid][pAsesinatos]++;

        // Formatear y ejecutar la consulta de actualización
        new query[256];
        format(query, sizeof(query), "UPDATE Jugadores SET pAsesinatos = %d, pScore = %d, pDinero = %d WHERE pNombre = '%s'",
               PlayerInfo[killerid][pAsesinatos],
               PlayerInfo[killerid][pScore],
               PlayerInfo[killerid][pDinero],
               PlayerInfo[killerid][pNombre]);
        db_query(database, query);

        // Verificar si la racha es un múltiplo de 5
        if (PlayerInfo[killerid][pRachas] % 5 == 0)
        {
            // Mostrar mensaje a todos los jugadores en verde oscuro
            new message[128];
            new name[MAX_PLAYER_NAME];
            GetPlayerName(killerid, name, sizeof(name));
            format(message, sizeof(message), "%s has reached %d kills without dying", name, PlayerInfo[killerid][pRachas]);
            SendClientMessageToAll(0x008000FF, message); // Verde oscuro
        }
        // Mensajes personalizados con TextDraws
        new killerName[MAX_PLAYER_NAME], victimName[MAX_PLAYER_NAME];
        GetPlayerName(killerid, killerName, sizeof(killerName));
        GetPlayerName(playerid, victimName, sizeof(victimName));

        // Mensaje para el asesino
        new killerMessage[128];
        format(killerMessage, sizeof(killerMessage), "~w~You have killed ~r~%s", victimName);
        PlayerTextDrawSetString(killerid, DeathTextDraw[killerid], killerMessage);
        PlayerTextDrawShow(killerid, DeathTextDraw[killerid]);

        // Mensaje para la víctima
        new victimMessage[128];
        format(victimMessage, sizeof(victimMessage), "~w~It has killed you ~r~%s", killerName);
        PlayerTextDrawSetString(playerid, DeathTextDraw[playerid], victimMessage);
        PlayerTextDrawShow(playerid, DeathTextDraw[playerid]);

        // Ocultar los TextDraws después de 5 segundos
        SetTimerEx("HideDeathTextDraw", 5000, false, "ii", killerid, playerid);
    }

    // Restablecer la racha de asesinatos del jugador que murió
    PlayerInfo[playerid][pRachas] = 0;

    // Incrementar las estadísticas de muertes del jugador que murió
    PlayerInfo[playerid][pMuertes]++;

    // Formatear y ejecutar la consulta de actualización para muertes
    new query[256];
    format(query, sizeof(query), "UPDATE Jugadores SET pMuertes = %d WHERE pNombre = '%s'",
           PlayerInfo[playerid][pMuertes],
           PlayerInfo[playerid][pNombre]);
    db_query(database, query);

    SendDeathMessage(killerid, playerid, reason);

    return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    return 1;
}

public OnVehicleSpawn(vehicleid)
{
    return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
    return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
    return 1;
}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
    return 1;
}

public OnPlayerText(playerid, text[])
{
    //Discord
    new name[MAX_PLAYER_NAME + 1], DCC_Channel:serverChatChannel = DCC_FindChannelByName("moderator-only");
    GetPlayerName(playerid, name, sizeof(name));
    new msg[256]; // Aumentamos el tamaño para prevenir desbordamientos
    new convertedText[256]; // Buffer para el texto convertido
    utf8encode(convertedText, text, sizeof(convertedText)); // Convierte caracteres especiales
    format(msg, sizeof(msg), ":speech_left: **%s[%d]:** %s", name, playerid, convertedText);
    DCC_SendChannelMessage(serverChatChannel, msg);
    //Discord

    // Verificar si el jugador está muteado
    if (PlayerInfo[playerid][pSilenciado] == 1 && PlayerInfo[playerid][pTiempoSilenciado] > gettime())
    {
        new tiempoRestante = PlayerInfo[playerid][pTiempoSilenciado] - gettime();
        new minutosRestantes = tiempoRestante / 60;

        // Mensaje al jugador muteado
        new mensajeMuteado[128];
        format(mensajeMuteado, sizeof(mensajeMuteado), "You are muted. Remaining time: %d minutes.", minutosRestantes);
        SendClientMessage(playerid, COLOR_RED, mensajeMuteado);

        return 0; // Evitar que el jugador envíe mensajes
    }

    return 1;
}

public OnPlayerUpdate(playerid)
{
    SetTimer("UpdatePlayersFps", 1000, true);
    ActualizarTextDraws(playerid);
    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
    return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
    return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
    return 1;
}

public OnPlayerGiveDamageActor(playerid, damaged_actorid, Float:amount, WEAPON:weaponid, bodypart)
{
    return 1;
}

public OnActorStreamIn(actorid, forplayerid)
{
    return 1;
}

public OnActorStreamOut(actorid, forplayerid)
{
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch (dialogid)
    {
        case DIALOG_REGISTER:
        {
            if (!response) Kick(playerid); // Expulsar si no se registra
            if (strlen(inputtext) < 4 || strlen(inputtext) > 128)
            {
                SendClientMessage(playerid, COLOR_RED, "Password must be between 4 and 128 characters.");
                ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", "Please enter a valid password:", "Accept", "Cancel");
                return 1;
            }
            bcrypt_hash(playerid, "OnPasswordHashed", inputtext, BCRYPT_COST);
            return 1;
        }

        case DIALOG_LOGIN:
        {
            if (!response) Kick(playerid); // Expulsar si no inicia sesión
            bcrypt_verify(playerid, "OnPasswordVerified", inputtext, PlayerInfo[playerid][pPassword]);
            return 1;
        }
        case DIALOGO_REPORTES: // Diálogo de reportes
        {
            if (!response) // Si el jugador hace clic en "Cancelar"
            {
                SendClientMessage(playerid, COLOR_RED, "You have closed the reporting dialog.");
                return 1;
            }

            // Obtener el ID del reporte seleccionado
            new reporte_id = listitem + 1; // Los índices comienzan en 0, pero los IDs en 1

            // Verificar si el ID del reporte es válido
            if (reporte_id < 1 || reporte_id > Reporte_Count)
            {
                SendClientMessage(playerid, COLOR_RED, "Invalid report ID.");
                return 1;
            }

            // Eliminar el reporte
            for (new i = reporte_id - 1; i < Reporte_Count - 1; i++)
            {
                Reportes[i] = Reportes[i + 1];
            }
            Reporte_Count--;

            // Notificar al administrador
            new admin_message[128];
            format(admin_message, sizeof(admin_message), "You have closed the report #%d.", reporte_id);
            SendClientMessage(playerid, 0x00FF00FF, admin_message);

            // Notificar al jugador que envió el reporte
            new reporter_id = Reportes[reporte_id - 1][REPORT_PLAYER_ID];
            if (IsPlayerConnected(reporter_id))
            {
                SendClientMessage(reporter_id, 0x00FF00FF, "Your report has been reviewed and closed by an administrator.");
            }

            return 1;
        }
    }
    return 0;
}

public OnPlayerModelSelection(playerid, response, listid, modelid)
{
    if (listid == skinlist) // Verifica que la lista sea la de skins
    {
        if (response) // Si el jugador seleccionó un skin
        {
            SetPlayerSkinAndSave(playerid, modelid); // Aplica el skin seleccionado
        }
        else // Si el jugador canceló la selección
        {
            SendClientMessage(playerid, COLOR_RED, "You canceled the skin selection");
        }
        return 1;
    }
    return 0; // Si no es la lista de skins, no hacemos nada
}

public OnPlayerEnterGangZone(playerid, zoneid)
{
    return 1;
}

public OnPlayerLeaveGangZone(playerid, zoneid)
{
    return 1;
}

public OnPlayerEnterPlayerGangZone(playerid, zoneid)
{
    return 1;
}

public OnPlayerLeavePlayerGangZone(playerid, zoneid)
{
    return 1;
}

public OnPlayerClickGangZone(playerid, zoneid)
{
    return 1;
}

public OnPlayerClickPlayerGangZone(playerid, zoneid)
{
    return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
    return 1;
}

public OnPlayerExitedMenu(playerid)
{
    return 1;
}

public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
    return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
    return 1;
}

public OnPlayerFinishedDownloading(playerid, virtualworld)
{
    return 1;
}

public OnPlayerRequestDownload(playerid, DOWNLOAD_REQUEST:type, crc)
{
    return 1;
}

public OnRconCommand(cmd[])
{
    return 0;
}

public OnPlayerSelectObject(playerid, SELECT_OBJECT:type, objectid, modelid, Float:fX, Float:fY, Float:fZ)
{
    return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, EDIT_RESPONSE:response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
    return 1;
}

public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
    return 1;
}

public OnObjectMoved(objectid)
{
    return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
    return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
    return 1;
}

public OnPlayerPickUpPlayerPickup(playerid, pickupid)
{
    return 1;
}

public OnPickupStreamIn(pickupid, playerid)
{
    return 1;
}

public OnPickupStreamOut(pickupid, playerid)
{
    return 1;
}

public OnPlayerPickupStreamIn(pickupid, playerid)
{
    return 1;
}

public OnPlayerPickupStreamOut(pickupid, playerid)
{
    return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
    return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
    return 1;
}

public OnPlayerDamage(&playerid, &Float:amount, &issuerid, &WEAPON:weapon, &bodypart)
{
    return 1;
}

public OnPlayerDamageDone(playerid, Float:amount, issuerid, WEAPON:weapon, bodypart)
{
    if (IsPlayerConnected(playerid) && IsPlayerConnected(issuerid))
    {
        // Incrementar la experiencia del jugador atacante en función del daño causado
        new incrementoExperiencia = floatround(amount / 10.0);
        PlayerInfo[issuerid][pExperiencia] += incrementoExperiencia;

        // Verificar si el jugador ha alcanzado 100 puntos de experiencia para subir de nivel
        if (PlayerInfo[issuerid][pExperiencia] >= 100)
        {
            PlayerInfo[issuerid][pNivel] += 1;
            PlayerInfo[issuerid][pExperiencia] = 0; // Resetear la experiencia

            // Mensaje opcional para el jugador atacante
            new nombreJugador[MAX_PLAYER_NAME];
            GetPlayerName(issuerid, nombreJugador, sizeof(nombreJugador));
            new mensaje[128];
            format(mensaje, sizeof(mensaje), "You've leveled up! Now you are on the level %d.", PlayerInfo[issuerid][pNivel]);
            SendClientMessage(issuerid, COLOR_GREEN, mensaje);
        }
    }

    if (issuerid != INVALID_PLAYER_ID)
    {
        // Mostrar el daño en un texto 3D
        new Float:fOriginX, Float:fOriginY, Float:fOriginZ, Float:fHitPosX, Float:fHitPosY, Float:fHitPosZ, PlayerText3D:hitbar, string[10];
        format(string, 10, "%.0f", amount);
        GetPlayerLastShotVectors(issuerid, fOriginX, fOriginY, fOriginZ, fHitPosX, fHitPosY, fHitPosZ);
        hitbar = CreatePlayer3DTextLabel(issuerid, string, 0xFF0000FF, fHitPosX, fHitPosY, fHitPosZ, 70);
        SetTimerEx("Destruir3DTextLabelHit", 1200, false, "dd", issuerid, _:hitbar);

    }
    return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
    return 1;
}

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    return 1;
}

public OnIncomingConnection(playerid, ip_address[], port)
{
    return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
    return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    return 1;
}

public OnTrailerUpdate(playerid, vehicleid)
{
    return 1;
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
    return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
    return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
    return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
    return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
    return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
    return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
    return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
    return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
    return 1;
}


