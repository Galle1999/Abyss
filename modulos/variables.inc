// COLORES BÁSICOS
#define COLOR_RED 0xFF0000FF
#define COLOR_GREEN 0x00FF00FF
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_YELLOW 0xFFFF00FF
#define COLOR_BLUE 0x0000FFFF
#define COLOR_ORANGE 0xFFA500FF
#define COLOR_PURPLE 0x800080FF
#define COLOR_PINK 0xFFC0CBFF
#define COLOR_CYAN 0x00FFFFFF
#define COLOR_BLACK 0x000000FF
#define COLOR_GREY 0x999894AA

// Definición de los niveles de administrador
#define ADMIN_USUARIO 0
#define ADMIN_AYUDANTE 1
#define ADMIN_MODERADOR 2
#define ADMIN_ADMINISTRADOR 3
#define ADMIN_SUPERVISOR 4
#define ADMIN_OWNER 5

//Diálogos

#define DIALOG_LOGIN 1
#define DIALOG_REGISTER 2
#define DIALOG_CHANGE_PASSWORD 3
#define DIALOG_BAN 4
#define DIALOG_STATS 5
#define DIALOG_ADMINS 6
#define DIALOGO_REPORTES 7
#define DIALOG_CMDS 8
#define DIALOG_AYUDA 9
#define DIALOG_CREDITOS 10
#define DIALOG_ACMDS 11
#define DIALOG_MINIJUEGOS 12

//Base de datos
new DB:database;
// Data del usuario
enum PlayerData
{
    pID,
    pNombre[MAX_PLAYER_NAME],
    pPassword[129],
    pScore,
    pAsesinatos,
    pMuertes,
    pDinero,
    pSkin,
    pNivelAdmin,
    pNivel,
    pExperiencia,
    pSilenciado,
    pJail,
    pBaneado,
    pAdminBan[MAX_PLAYER_NAME],
    pMotivoBan[128],
    pMotivoSilenciado[128],
    pFechaBan[20],
    pIp[16],
    pRegistrado,
    pDuelosGanados,
    pDuelosPerdidos,
    pClima,
    pHora,
    pUltimaConexion[24],
    pPais[64],
    pTiempoSilenciado,
    pTiempoJail,
    pRachas,
    pTargetName[MAX_PLAYER_NAME]
};

// Variables globales
new PlayerInfo[MAX_PLAYERS][PlayerData];

new skinlist = mS_INVALID_LISTID;

//Textdraws variables
new Text:FPS;
new Text:porcentaje;
new Text:Ping;
new Text:Pingporcentaje;
new Text:Packetloss;
new Text:Packetlossporcentaje;
//
new Text:logoTextDraw;
new Text:FechaTextdraw;
new tiempoActual[128];
//
// Textdraws de estadisticas
new PlayerText:TextDrawScore[MAX_PLAYERS];
new PlayerText:TextDrawScorePorcentaje[MAX_PLAYERS];
new PlayerText:TextDrawAsesinatos[MAX_PLAYERS];
new PlayerText:TextDrawAsesinatosPorcentaje[MAX_PLAYERS];
new PlayerText:TextDrawMuertes[MAX_PLAYERS];
new PlayerText:TextDrawMuertesPorcentaje[MAX_PLAYERS];
new bool:textdrawVisible[MAX_PLAYERS];
