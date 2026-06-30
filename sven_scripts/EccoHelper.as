//////////////////////////////////////////
// Ecco give item
// .give
//////////////////////////////////////////
namespace Give {
    bool CallBack(CEccoPlayerStorage@ const pStorage, CEccoMenuExcutor@ const pItem, CTCLArgList@ const pParams){
        CBasePlayer@ pPlayer = pStorage.GetPlayer();
        if(pPlayer is null || !pPlayer.IsNetClient())
            return false;
        string weapon;
        pParams[0].GetString(weapon);
        CBaseEntity@ pGive = g_EntityFuncs.Create(weapon, pPlayer.GetOrigin(), g_vecZero, true);
        pGive.pev.spawnflags |= 1024;
        pGive.KeyValue("m_flCustomRespawnTime", "-1");
        g_EntityFuncs.DispatchSpawn(pGive.edict());
        pGive.Touch(pPlayer);
        return true;
    }
}

void PluginInit(){
	g_Module.ScriptInfo.SetAuthor( "drabc" );
	g_Module.ScriptInfo.SetContactInfo( "bruh" );

    g_EccoFuncs.RegisterTCLFunction("AS::give", "int player, int menu, string weapon", "give a weapon", @Give::CallBack);
}

void PluginExit(){
    g_EccoFuncs.UnregisteTCLFunction("AS::give");
}