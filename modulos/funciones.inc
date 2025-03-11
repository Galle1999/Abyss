stock SetPlayerSkinAndSave(playerid, skinid)
{
    // Aplicar el skin al jugador
    SetPlayerSkin(playerid, skinid);

    // Guardar el skin en la base de datos
    PlayerInfo[playerid][pSkin] = skinid;
    new query[256];
    format(query, sizeof(query),
        "UPDATE Jugadores SET pSkin = %d WHERE pID = %d",
        PlayerInfo[playerid][pSkin],
        PlayerInfo[playerid][pID]
    );
    db_query(database, query);

    // Mensaje de confirmación para el jugador
    new mensaje[64];
    format(mensaje, sizeof(mensaje), "You have changed your skin to %d", skinid);
    SendClientMessage(playerid, COLOR_GREEN, mensaje);

    return 1;
}

forward KickPlayerDelayed(playerid);
public KickPlayerDelayed(playerid)
{
    if (IsPlayerConnected(playerid)) // Verificar si el jugador sigue conectado
    {
        Kick(playerid); // Expulsar al jugador
    }
    return 1;
}

forward ActualizarTextDraws(playerid);
public ActualizarTextDraws(playerid)
{
    // Obtener los valores de ping y packetloss del jugador
    new ping = GetPlayerPing(playerid);
    new Float:packetloss = NetStats_PacketLossPercent(playerid); // Usar NetStats_PacketLossPercent para obtener el packetloss

    // Obtener los FPS del jugador usando la función GetPlayerFPS
    new fps = GetPlayerFPS(playerid);

    // Actualizar los TextDraws con los valores obtenidos
    new fpsText[32], pingText[32], packetlossText[32];
    format(fpsText, sizeof(fpsText), "%d", fps);
    format(pingText, sizeof(pingText), "%d", ping);
    format(packetlossText, sizeof(packetlossText), "%d%%", packetloss);

    TextDrawSetString(porcentaje, fpsText);
    TextDrawSetString(Pingporcentaje, pingText);
    TextDrawSetString(Packetlossporcentaje, packetlossText);

    // Actualizar PlayerTextDraw de Score Porcentaje
    new scorePorcentajeText[32];
    format(scorePorcentajeText, sizeof(scorePorcentajeText), "%d", PlayerInfo[playerid][pScore]);
    PlayerTextDrawSetString(playerid, TextDrawScorePorcentaje[playerid], scorePorcentajeText);

    // Actualizar PlayerTextDraw de Asesinatos Porcentaje
    new asesinatosPorcentajeText[32];
    format(asesinatosPorcentajeText, sizeof(asesinatosPorcentajeText), "%d", PlayerInfo[playerid][pAsesinatos]);
    PlayerTextDrawSetString(playerid, TextDrawAsesinatosPorcentaje[playerid], asesinatosPorcentajeText);

    // Actualizar PlayerTextDraw de Muertes Porcentaje
    new muertesPorcentajeText[32];
    format(muertesPorcentajeText, sizeof(muertesPorcentajeText), "%d", PlayerInfo[playerid][pMuertes]);
    PlayerTextDrawSetString(playerid, TextDrawMuertesPorcentaje[playerid], muertesPorcentajeText);

    return 1;
}

static s_PlayerLastDrunkLevel[MAX_PLAYERS];
static s_PlayerFps[MAX_PLAYERS];

forward UpdatePlayersFps();
public UpdatePlayersFps()
{
    new playerDrunkLevel, currentFps;

    foreach (new i : Player)
    {
        playerDrunkLevel = GetPlayerDrunkLevel(i);

        if (playerDrunkLevel < 100)
        {
            SetPlayerDrunkLevel(i, 2000);
        }
        else if (s_PlayerLastDrunkLevel[i] != playerDrunkLevel)
        {
            currentFps = s_PlayerLastDrunkLevel[i] - playerDrunkLevel;
            s_PlayerLastDrunkLevel[i] = playerDrunkLevel;

            if (1 <= currentFps < 450)
            {
                s_PlayerFps[i] = currentFps;
            }
        }
    }
}

forward ActualizarFechaHoraTextDraw();
public ActualizarFechaHoraTextDraw()
{
    new dia, mes, anio, hora, minuto;
    getdate(dia, mes, anio); // Obtener la fecha actual
    gettime(hora, minuto); // Obtener la hora actual
    format(tiempoActual, sizeof(tiempoActual), "%02d.%02d.%d\n%02d.%02d", dia, mes, anio, hora, minuto); // Formatear la fecha y hora
    TextDrawSetString(FechaTextdraw, tiempoActual); // Establecer el texto en el TextDraw

    return 1;
}

GetPlayerFPS(playerid)
{
    if (!IsPlayerConnected(playerid))
    {
        return -1;
    }
    return s_PlayerFps[playerid];
}

stock TimestampToDate(timestamp, &year, &month, &day, &hour, &minute, &second)
{
    new const days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    new days = timestamp / 86400; // Días desde 1970-01-01
    year = 1970;

    // Calcular el año
    while (days >= 365)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            if (days >= 366)
            {
                days -= 366;
                year++;
            }
            else break;
        }
        else
        {
            days -= 365;
            year++;
        }
    }

    // Calcular el mes y el día
    month = 0;
    while (days >= days_in_month[month])
    {
        if (month == 1 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        {
            if (days >= 29)
            {
                days -= 29;
                month++;
            }
            else break;
        }
        else
        {
            days -= days_in_month[month];
            month++;
        }
    }
    day = days + 1;

    // Calcular la hora, minuto y segundo
    new remaining_seconds = timestamp % 86400;
    hour = remaining_seconds / 3600;
    remaining_seconds %= 3600;
    minute = remaining_seconds / 60;
    second = remaining_seconds % 60;
}

forward Destruir3DTextLabelHit(issuerid, PlayerText3D:hitbar);
public Destruir3DTextLabelHit(issuerid, PlayerText3D:hitbar)
{
    DeletePlayer3DTextLabel(issuerid, hitbar);
    return 1;
}

stock bool:IsNumeric(const str[])
{
    for (new i = 0; i < strlen(str); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false; // Retorna false si encuentra un carácter no numérico
        }
    }
    return true; // Retorna true si todos los caracteres son numéricos
}

forward EnviarMensajeBienvenida(playerid);
public EnviarMensajeBienvenida(playerid)
{
    // Verificar si el jugador sigue conectado y no está baneado
    if (IsPlayerConnected(playerid) && PlayerInfo[playerid][pBaneado] == 0)
    {
        new pname[MAX_PLAYER_NAME];
        GetPlayerName(playerid, pname, sizeof(pname));

        // Mensaje privado de bienvenida
        new welcomeMsg[256];
        format(welcomeMsg, sizeof(welcomeMsg), "{FFFF00}Welcome, %s, to {0000FF}Abyss Deathmatch{FFFF00}!.\nUse /help for more information.", pname);
        SendClientMessage(playerid, 0xFFFF00FF, welcomeMsg);
    }
    return 1;
}

stock ActualizarEstadoDelBot()
{
    new onlinePlayers = 0;

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i))
        {
            onlinePlayers++;
        }
    }

    new estado[64];
    format(estado, sizeof(estado), "Abyss - online: %d", onlinePlayers);
    DCC_SetBotActivity(estado);

    return 1;
}

// Sección de comandos Discord
forward OnDiscordJugadoresCommand(DCC_Interaction:interaction);
public OnDiscordJugadoresCommand(DCC_Interaction:interaction)
{
    new players[512], pname[MAX_PLAYER_NAME];
    new count = 0;

    // Obtener la lista de jugadores conectados con ID, Score y Ping
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i))
        {
            GetPlayerName(i, pname, sizeof(pname));
            new score = GetPlayerScore(i);
            new ping = GetPlayerPing(i);

            // Agregar la información del jugador al mensaje
            format(players, sizeof(players), "%sID: %d | Nick: %s | Score: %d | Ping: %d\n", players, i, pname, score, ping);
            count++;
        }
    }

    // Si no hay jugadores conectados, envía un mensaje
    if (count == 0)
    {
        DCC_SendInteractionMessage(interaction, "There are no players online at the moment.");
        return 1;
    }

    // Construir el mensaje con los jugadores conectados
    new message[600];
    format(message, sizeof(message), "Connected players (%d):\n%s", count, players);

    // Verifica que el mensaje no exceda el límite de Discord
    if (strlen(message) > 2000) // Si excede, envía un mensaje genérico
    {
        DCC_SendInteractionMessage(interaction, "The list of online players is too long to display.");
    }
    else
    {
        DCC_SendInteractionMessage(interaction, message);
    }
    return 1;
}

forward OnDiscordIPCommand(DCC_Interaction:interaction);
public OnDiscordIPCommand(DCC_Interaction:interaction)
{
    DCC_SendInteractionMessage(interaction, "203.0.113.42");
    return 1;
}


forward DCC_OnMessageCreate(DCC_Message:message);
public DCC_OnMessageCreate(DCC_Message:message)
{
    new realMsg[100];
    DCC_GetMessageContent(message, realMsg, 100);

    new convertedMsg[256];
    utf8decode(convertedMsg, realMsg, sizeof(convertedMsg)); // Convierte caracteres a UTF-8

    // Evitar menciones de usuarios excepto here y everyone
    new i = 0;
    while (convertedMsg[i] != '\0')
    {
        if (convertedMsg[i] == '@' &&
                strfind(convertedMsg[i], "@here", true) != i &&
                strfind(convertedMsg[i], "@everyone", true) != i)
        {
            convertedMsg[i] = '*'; // Reemplazar '@' con '*'
        }
        i++;
    }

    new bool:IsBot;
    new DCC_Channel:channel;
    DCC_GetMessageChannel(message, channel);
    new DCC_User:author;
    DCC_GetMessageAuthor(message, author);
    DCC_IsUserBot(author, IsBot);
    new DCC_Channel:serverChatChannel = DCC_FindChannelByName("moderador-only");
    new DCC_Guild:g_Guild = DCC_FindGuildById("1338671980030857227");
    if (channel == serverChatChannel && !IsBot) // Si es un bot no se mostrará en el chat
    {
        new user_name[32 + 1], str[152], role_name[32];
        DCC_GetUserName(author, user_name, 32);

        // Obtener el primer rol del usuario
        new DCC_Role:role;
        new role_count;
        DCC_GetGuildMemberRoleCount(g_Guild, author, role_count);

        if (role_count > 0)
        {
            DCC_GetGuildMemberRole(g_Guild, author, 0, role);
            DCC_GetRoleName(role, role_name, sizeof(role_name));
        }
        else
        {
            strcat(role_name, "Sin rol", sizeof(role_name));
        }

        format(str, sizeof(str), "Discord:{FF0000} %s {9b9b9b}[%s]: {ffffff}%s", user_name, role_name, convertedMsg);
        SendClientMessageToAll(-1, str);
    }
    return 1;
}