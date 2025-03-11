NetstatsTextdraw(playerid)
{
    FPS = TextDrawCreate(505.000000, 5.000000, "Fps");
    TextDrawFont(FPS, 3);
    TextDrawLetterSize(FPS, 0.183332, 1.000000);
    TextDrawTextSize(FPS, 400.000000, 17.000000);
    TextDrawSetOutline(FPS, 1);
    TextDrawSetShadow(FPS, 0);
    TextDrawAlignment(FPS, 1);
    TextDrawColor(FPS, 1097458175);
    TextDrawBackgroundColor(FPS, 255);
    TextDrawBoxColor(FPS, -206);
    TextDrawUseBox(FPS, 0);
    TextDrawSetProportional(FPS, 1);
    TextDrawSetSelectable(FPS, 0);
    TextDrawShowForPlayer(playerid, FPS);

    porcentaje = TextDrawCreate(519.000000, 5.000000, "0");
    TextDrawFont(porcentaje, 3);
    TextDrawLetterSize(porcentaje, 0.183332, 1.000000);
    TextDrawTextSize(porcentaje, 400.000000, 17.000000);
    TextDrawSetOutline(porcentaje, 1);
    TextDrawSetShadow(porcentaje, 0);
    TextDrawAlignment(porcentaje, 1);
    TextDrawColor(porcentaje, -1);
    TextDrawBackgroundColor(porcentaje, 255);
    TextDrawBoxColor(porcentaje, -206);
    TextDrawUseBox(porcentaje, 0);
    TextDrawSetProportional(porcentaje, 1);
    TextDrawSetSelectable(porcentaje, 0);
    TextDrawShowForPlayer(playerid, porcentaje);

    Ping = TextDrawCreate(540.000000, 5.000000, "Ping");
    TextDrawFont(Ping, 3);
    TextDrawLetterSize(Ping, 0.183332, 1.000000);
    TextDrawTextSize(Ping, 400.000000, 17.000000);
    TextDrawSetOutline(Ping, 1);
    TextDrawSetShadow(Ping, 0);
    TextDrawAlignment(Ping, 1);
    TextDrawColor(Ping, 1097458175);
    TextDrawBackgroundColor(Ping, 255);
    TextDrawBoxColor(Ping, -206);
    TextDrawUseBox(Ping, 0);
    TextDrawSetProportional(Ping, 1);
    TextDrawSetSelectable(Ping, 0);
    TextDrawShowForPlayer(playerid, Ping);

    Pingporcentaje = TextDrawCreate(556.000000, 5.000000, "0");
    TextDrawFont(Pingporcentaje, 3);
    TextDrawLetterSize(Pingporcentaje, 0.183332, 1.000000);
    TextDrawTextSize(Pingporcentaje, 400.000000, 17.000000);
    TextDrawSetOutline(Pingporcentaje, 1);
    TextDrawSetShadow(Pingporcentaje, 0);
    TextDrawAlignment(Pingporcentaje, 1);
    TextDrawColor(Pingporcentaje, -1);
    TextDrawBackgroundColor(Pingporcentaje, 255);
    TextDrawBoxColor(Pingporcentaje, -206);
    TextDrawUseBox(Pingporcentaje, 0);
    TextDrawSetProportional(Pingporcentaje, 1);
    TextDrawSetSelectable(Pingporcentaje, 0);
    TextDrawShowForPlayer(playerid, Pingporcentaje);

    Packetloss = TextDrawCreate(577.000000, 5.000000, "Packetloss");
    TextDrawFont(Packetloss, 3);
    TextDrawLetterSize(Packetloss, 0.183332, 1.000000);
    TextDrawTextSize(Packetloss, 400.000000, 17.000000);
    TextDrawSetOutline(Packetloss, 1);
    TextDrawSetShadow(Packetloss, 0);
    TextDrawAlignment(Packetloss, 1);
    TextDrawColor(Packetloss, 1097458175);
    TextDrawBackgroundColor(Packetloss, 255);
    TextDrawBoxColor(Packetloss, -206);
    TextDrawUseBox(Packetloss, 0);
    TextDrawSetProportional(Packetloss, 1);
    TextDrawSetSelectable(Packetloss, 0);
    TextDrawShowForPlayer(playerid, Packetloss);

    Packetlossporcentaje = TextDrawCreate(587.000000, 5.000000, "0");
    TextDrawFont(Packetlossporcentaje, 3);
    TextDrawLetterSize(Packetlossporcentaje, 0.183332, 1.000000);
    TextDrawTextSize(Packetlossporcentaje, 400.000000, 17.000000);
    TextDrawSetOutline(Packetlossporcentaje, 1);
    TextDrawSetShadow(Packetlossporcentaje, 0);
    TextDrawAlignment(Packetlossporcentaje, 1);
    TextDrawColor(Packetlossporcentaje, -1);
    TextDrawBackgroundColor(Packetlossporcentaje, 255);
    TextDrawBoxColor(Packetlossporcentaje, -206);
    TextDrawUseBox(Packetlossporcentaje, 0);
    TextDrawSetProportional(Packetlossporcentaje, 1);
    TextDrawSetSelectable(Packetlossporcentaje, 0);
    TextDrawShowForPlayer(playerid, Packetlossporcentaje);
}

logo(playerid)
{

    logoTextDraw = TextDrawCreate(575.000000, 24.000000, "_ABYSS DEATHMATCH");
    TextDrawLetterSize(logoTextDraw, 0.18, 0.81);
    TextDrawAlignment(logoTextDraw, 2);
    TextDrawColor(logoTextDraw, -1);
    TextDrawSetOutline(logoTextDraw, 1);
    TextDrawBackgroundColor(logoTextDraw, 80);
    TextDrawFont(logoTextDraw, 1);
    TextDrawSetProportional(logoTextDraw, 1);
    TextDrawShowForPlayer(playerid, logoTextDraw);
}

fechatextdraw(playerid)
{
    FechaTextdraw = TextDrawCreate(577.000000, 53.000000, " ");
    TextDrawLetterSize(FechaTextdraw, 0.18, 0.81);
    TextDrawAlignment(FechaTextdraw, 2);
    TextDrawColor(FechaTextdraw, -1);
    TextDrawSetOutline(FechaTextdraw, 1);
    TextDrawBackgroundColor(FechaTextdraw, 80);
    TextDrawFont(FechaTextdraw, 1);
    TextDrawSetProportional(FechaTextdraw, 1);
    TextDrawShowForPlayer(playerid, FechaTextdraw);
}


CreatePlayerStatsTextDraws(playerid)
{
    TextDrawAsesinatos[playerid] = CreatePlayerTextDraw(playerid, 591.000000, 385.000000, "Kills:");
    PlayerTextDrawFont(playerid, TextDrawAsesinatos[playerid], 1);
    PlayerTextDrawLetterSize(playerid, TextDrawAsesinatos[playerid], 0.241666, 1.049998);
    PlayerTextDrawTextSize(playerid, TextDrawAsesinatos[playerid], 400.000000, 17.000000);
    PlayerTextDrawSetOutline(playerid, TextDrawAsesinatos[playerid], 0);
    PlayerTextDrawSetShadow(playerid, TextDrawAsesinatos[playerid], 1);
    PlayerTextDrawAlignment(playerid, TextDrawAsesinatos[playerid], 1);
    PlayerTextDrawColor(playerid, TextDrawAsesinatos[playerid], 1097458175);
    PlayerTextDrawBackgroundColor(playerid, TextDrawAsesinatos[playerid], 255);
    PlayerTextDrawBoxColor(playerid, TextDrawAsesinatos[playerid], 50);
    PlayerTextDrawUseBox(playerid, TextDrawAsesinatos[playerid], 0);
    PlayerTextDrawSetProportional(playerid, TextDrawAsesinatos[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, TextDrawAsesinatos[playerid], 0);
    PlayerTextDrawShow(playerid, TextDrawAsesinatos[playerid]);

    new asesinatosText[32];
    format(asesinatosText, sizeof(asesinatosText), "%d", PlayerInfo[playerid][pAsesinatos]);
    TextDrawAsesinatosPorcentaje[playerid] = CreatePlayerTextDraw(playerid, 611.000000, 385.000000, asesinatosText);
    PlayerTextDrawFont(playerid, TextDrawAsesinatosPorcentaje[playerid], 1);
    PlayerTextDrawLetterSize(playerid, TextDrawAsesinatosPorcentaje[playerid], 0.241666, 1.049998);
    PlayerTextDrawTextSize(playerid, TextDrawAsesinatosPorcentaje[playerid], 400.000000, 17.000000);
    PlayerTextDrawSetOutline(playerid, TextDrawAsesinatosPorcentaje[playerid], 0);
    PlayerTextDrawSetShadow(playerid, TextDrawAsesinatosPorcentaje[playerid], 1);
    PlayerTextDrawAlignment(playerid, TextDrawAsesinatosPorcentaje[playerid], 1);
    PlayerTextDrawColor(playerid, TextDrawAsesinatosPorcentaje[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, TextDrawAsesinatosPorcentaje[playerid], 255);
    PlayerTextDrawBoxColor(playerid, TextDrawAsesinatosPorcentaje[playerid], 50);
    PlayerTextDrawUseBox(playerid, TextDrawAsesinatosPorcentaje[playerid], 0);
    PlayerTextDrawSetProportional(playerid, TextDrawAsesinatosPorcentaje[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, TextDrawAsesinatosPorcentaje[playerid], 0);
    PlayerTextDrawShow(playerid, TextDrawAsesinatosPorcentaje[playerid]);

    TextDrawMuertes[playerid] = CreatePlayerTextDraw(playerid, 582.000000, 394.000000, "Deaths:");
    PlayerTextDrawFont(playerid, TextDrawMuertes[playerid], 1);
    PlayerTextDrawLetterSize(playerid, TextDrawMuertes[playerid], 0.241666, 1.049998);
    PlayerTextDrawTextSize(playerid, TextDrawMuertes[playerid], 400.000000, 17.000000);
    PlayerTextDrawSetOutline(playerid, TextDrawMuertes[playerid], 0);
    PlayerTextDrawSetShadow(playerid, TextDrawMuertes[playerid], 1);
    PlayerTextDrawAlignment(playerid, TextDrawMuertes[playerid], 1);
    PlayerTextDrawColor(playerid, TextDrawMuertes[playerid], 1097458175);
    PlayerTextDrawBackgroundColor(playerid, TextDrawMuertes[playerid], 255);
    PlayerTextDrawBoxColor(playerid, TextDrawMuertes[playerid], 50);
    PlayerTextDrawUseBox(playerid, TextDrawMuertes[playerid], 0);
    PlayerTextDrawSetProportional(playerid, TextDrawMuertes[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, TextDrawMuertes[playerid], 0);
    PlayerTextDrawShow(playerid, TextDrawMuertes[playerid]);

    new muertesText[32];
    format(muertesText, sizeof(muertesText), "%d", PlayerInfo[playerid][pMuertes]);
    TextDrawMuertesPorcentaje[playerid] = CreatePlayerTextDraw(playerid, 611.000000, 394.000000, muertesText);
    PlayerTextDrawFont(playerid, TextDrawMuertesPorcentaje[playerid], 1);
    PlayerTextDrawLetterSize(playerid, TextDrawMuertesPorcentaje[playerid], 0.241666, 1.049998);
    PlayerTextDrawTextSize(playerid, TextDrawMuertesPorcentaje[playerid], 400.000000, 17.000000);
    PlayerTextDrawSetOutline(playerid, TextDrawMuertesPorcentaje[playerid], 0);
    PlayerTextDrawSetShadow(playerid, TextDrawMuertesPorcentaje[playerid], 1);
    PlayerTextDrawAlignment(playerid, TextDrawMuertesPorcentaje[playerid], 1);
    PlayerTextDrawColor(playerid, TextDrawMuertesPorcentaje[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, TextDrawMuertesPorcentaje[playerid], 255);
    PlayerTextDrawBoxColor(playerid, TextDrawMuertesPorcentaje[playerid], 50);
    PlayerTextDrawUseBox(playerid, TextDrawMuertesPorcentaje[playerid], 0);
    PlayerTextDrawSetProportional(playerid, TextDrawMuertesPorcentaje[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, TextDrawMuertesPorcentaje[playerid], 0);
    PlayerTextDrawShow(playerid, TextDrawMuertesPorcentaje[playerid]);

    TextDrawScore[playerid] = CreatePlayerTextDraw(playerid, 586.000000, 403.000000, "Score:");
    PlayerTextDrawFont(playerid, TextDrawScore[playerid], 1);
    PlayerTextDrawLetterSize(playerid, TextDrawScore[playerid], 0.241666, 1.049998);
    PlayerTextDrawTextSize(playerid, TextDrawScore[playerid], 400.000000, 17.000000);
    PlayerTextDrawSetOutline(playerid, TextDrawScore[playerid], 0);
    PlayerTextDrawSetShadow(playerid, TextDrawScore[playerid], 1);
    PlayerTextDrawAlignment(playerid, TextDrawScore[playerid], 1);
    PlayerTextDrawColor(playerid, TextDrawScore[playerid], 1097458175);
    PlayerTextDrawBackgroundColor(playerid, TextDrawScore[playerid], 255);
    PlayerTextDrawBoxColor(playerid, TextDrawScore[playerid], 50);
    PlayerTextDrawUseBox(playerid, TextDrawScore[playerid], 0);
    PlayerTextDrawSetProportional(playerid, TextDrawScore[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, TextDrawScore[playerid], 0);
    PlayerTextDrawShow(playerid, TextDrawScore[playerid]);

    new scoreText[32];
    format(scoreText, sizeof(scoreText), "%d", PlayerInfo[playerid][pScore]);
    TextDrawScorePorcentaje[playerid] = CreatePlayerTextDraw(playerid, 611.000000, 403.000000, scoreText);
    PlayerTextDrawFont(playerid, TextDrawScorePorcentaje[playerid], 1);
    PlayerTextDrawLetterSize(playerid, TextDrawScorePorcentaje[playerid], 0.241666, 1.049998);
    PlayerTextDrawTextSize(playerid, TextDrawScorePorcentaje[playerid], 400.000000, 17.000000);
    PlayerTextDrawSetOutline(playerid, TextDrawScorePorcentaje[playerid], 0);
    PlayerTextDrawSetShadow(playerid, TextDrawScorePorcentaje[playerid], 1);
    PlayerTextDrawAlignment(playerid, TextDrawScorePorcentaje[playerid], 1);
    PlayerTextDrawColor(playerid, TextDrawScorePorcentaje[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, TextDrawScorePorcentaje[playerid], 255);
    PlayerTextDrawBoxColor(playerid, TextDrawScorePorcentaje[playerid], 50);
    PlayerTextDrawUseBox(playerid, TextDrawScorePorcentaje[playerid], 0);
    PlayerTextDrawSetProportional(playerid, TextDrawScorePorcentaje[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, TextDrawScorePorcentaje[playerid], 0);
    PlayerTextDrawShow(playerid, TextDrawScorePorcentaje[playerid]);
}


TextdrawKills(playerid)
{
    // Crear el TextDraw para el jugador
    DeathTextDraw[playerid] = CreatePlayerTextDraw(playerid, 320.0, 350.0, " "); // Posición (x, y) y texto inicial vacío
    PlayerTextDrawAlignment(playerid, DeathTextDraw[playerid], 2); // Centrar el TextDraw
    PlayerTextDrawBackgroundColor(playerid, DeathTextDraw[playerid], 0x000000FF); // Color de fondo (negro)
    PlayerTextDrawFont(playerid, DeathTextDraw[playerid], 2); // Fuente del TextDraw
    PlayerTextDrawLetterSize(playerid, DeathTextDraw[playerid], 0.5, 2.0); // Tamaño de las letras
    PlayerTextDrawColor(playerid, DeathTextDraw[playerid], 0xba0000AA); // Color del texto (blanco)
    PlayerTextDrawSetOutline(playerid, DeathTextDraw[playerid], 1); // Borde del texto
    PlayerTextDrawSetProportional(playerid, DeathTextDraw[playerid], 1); // Texto proporcional
    PlayerTextDrawTextSize(playerid, DeathTextDraw[playerid], 0.0, 200.0); // Tamaño del cuadro de fondo
}
