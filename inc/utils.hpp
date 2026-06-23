#include <string>

namespace IRC {
    enum Code {
        // Welcome (001–004)
        RPL_WELCOME           = 1,
        RPL_YOURHOST          = 2,
        RPL_CREATED           = 3,
        RPL_MYINFO            = 4,

        // ISUPPORT (005)
        RPL_ISUPPORT          = 5,

        // Server (200–269)
        RPL_TRACELINK         = 200,
        RPL_TRACECONNECTING   = 201,
        RPL_TRACEHANDSHAKE    = 202,
        RPL_TRACEUNKNOWN      = 203,
        RPL_TRACEOPERATOR     = 204,
        RPL_TRACEUSER         = 205,
        RPL_TRACESERVER       = 206,
        RPL_TRACESERVICE      = 207,
        RPL_TRACENEWTYPE      = 208,
        RPL_TRACECLASS        = 209,
        RPL_TRACERECONNECT    = 210,
        RPL_STATSLINKINFO     = 211,
        RPL_STATSCOMMANDS     = 212,
        RPL_STATSCLINE        = 213,
        RPL_STATSNLINE        = 214,
        RPL_STATSILINE        = 215,
        RPL_STATSKLINE        = 216,
        RPL_STATSQLINE        = 217,
        RPL_STATSYLINE        = 218,
        RPL_ENDOFSTATS        = 219,
        RPL_UMODEIS           = 221,
        RPL_SERVICEINFO       = 231,
        RPL_ENDOFSERVICES     = 232,
        RPL_SERVICE           = 233,
        RPL_SERVLIST          = 234,
        RPL_SERVLISTEND       = 235,
        RPL_STATSVLINE        = 240,
        RPL_STATSLLINE        = 241,
        RPL_STATSUPTIME       = 242,
        RPL_STATSOLINE        = 243,
        RPL_STATSHLINE        = 244,
        RPL_STATSSLINE        = 245,
        RPL_STATSPING         = 246,
        RPL_STATSBLINE        = 247,
        RPL_STATSDLINE        = 250,
        RPL_LUSERCLIENT       = 251,
        RPL_LUSEROP           = 252,
        RPL_LUSERUNKNOWN      = 253,
        RPL_LUSERCHANNELS     = 254,
        RPL_LUSERME           = 255,
        RPL_ADMINME           = 256,
        RPL_ADMINLOC1         = 257,
        RPL_ADMINLOC2         = 258,
        RPL_ADMINEMAIL        = 259,
        RPL_TRACELOG          = 261,
        RPL_TRACEEND          = 262,
        RPL_TRYAGAIN          = 263,

        // Channels / Users (300–399)
        RPL_NONE              = 300,
        RPL_AWAY              = 301,
        RPL_USERHOST          = 302,
        RPL_ISON              = 303,
        RPL_UNAWAY            = 305,
        RPL_NOWAWAY           = 306,
        RPL_WHOISUSER         = 311,
        RPL_WHOISSERVER       = 312,
        RPL_WHOISOPERATOR     = 313,
        RPL_WHOWASUSER        = 314,
        RPL_ENDOFWHO          = 315,
        RPL_WHOISCHANOP       = 316,
        RPL_WHOISIDLE         = 317,
        RPL_ENDOFWHOIS        = 318,
        RPL_WHOISCHANNELS     = 319,
        RPL_LISTSTART         = 321,
        RPL_LIST              = 322,
        RPL_LISTEND           = 323,
        RPL_CHANNELMODEIS     = 324,
        RPL_UNIQOPIS          = 325,
        RPL_NOTOPIC           = 331,
        RPL_TOPIC             = 332,
        RPL_INVITING          = 341,
        RPL_SUMMONING         = 342,
        RPL_INVITELIST        = 346,
        RPL_ENDOFINVITELIST   = 347,
        RPL_EXCEPTLIST        = 348,
        RPL_ENDOFEXCEPTLIST   = 349,
        RPL_VERSION           = 351,
        RPL_WHOREPLY          = 352,
        RPL_NAMREPLY          = 353,
        RPL_KILLDONE          = 361,
        RPL_CLOSING           = 362,
        RPL_CLOSEEND          = 363,
        RPL_LINKS             = 364,
        RPL_ENDOFLINKS        = 365,
        RPL_ENDOFNAMES        = 366,
        RPL_BANLIST           = 367,
        RPL_ENDOFBANLIST      = 368,
        RPL_ENDOFWHOWAS       = 369,
        RPL_INFO              = 371,
        RPL_MOTD              = 372,
        RPL_INFOSTART         = 373,
        RPL_ENDOFINFO         = 374,
        RPL_MOTDSTART         = 375,
        RPL_ENDOFMOTD         = 376,
        RPL_YOUREOPER         = 381,
        RPL_REHASHING         = 382,
        RPL_YOURESERVICE      = 383,
        RPL_MYPORTIS          = 384,
        RPL_NOTOPERANYMORE    = 385,
        RPL_TIME              = 391,
        RPL_USERSSTART        = 392,
        RPL_USERS             = 393,
        RPL_ENDOFUSERS        = 394,
        RPL_NOUSERS           = 395,

        // Errors (400–502)
        ERR_NOSUCHNICK        = 401,
        ERR_NOSUCHSERVER      = 402,
        ERR_NOSUCHCHANNEL     = 403,
        ERR_CANNOTSENDTOCHAN  = 404,
        ERR_TOOMANYCHANNELS   = 405,
        ERR_WASNOSUCHNICK     = 406,
        ERR_TOOMANYTARGETS    = 407,
        ERR_NOSUCHSERVICE     = 408,
        ERR_NOORIGIN          = 409,
        ERR_NORECIPIENT       = 411,
        ERR_NOTEXTTOSEND      = 412,
        ERR_NOTOPLEVEL        = 413,
        ERR_WILDTOPLEVEL      = 414,
        ERR_BADMASK           = 415,
        ERR_UNKNOWNCOMMAND    = 421,
        ERR_NOMOTD            = 422,
        ERR_NOADMININFO       = 423,
        ERR_FILEERROR         = 424,
        ERR_NONICKNAMEGIVEN   = 431,
        ERR_ERRONEUSNICKNAME  = 432,
        ERR_NICKNAMEINUSE     = 433,
        ERR_NICKCOLLISION     = 436,
        ERR_UNAVAILRESOURCE   = 437,
        ERR_USERNOTINCHANNEL  = 441,
        ERR_NOTONCHANNEL      = 442,
        ERR_USERONCHANNEL     = 443,
        ERR_NOLOGIN           = 444,
        ERR_SUMMONDISABLED    = 445,
        ERR_USERSDISABLED     = 446,
        ERR_NOTREGISTERED     = 451,
        ERR_NEEDMOREPARAMS    = 461,
        ERR_ALREADYREGISTRED  = 462,
        ERR_NOPERMFORHOST     = 463,
        ERR_PASSWDMISMATCH    = 464,
        ERR_YOUREBANNEDCREEP  = 465,
        ERR_YOUWILLBEBANNED   = 466,
        ERR_KEYSET            = 467,
        ERR_CHANNELISFULL     = 471,
        ERR_UNKNOWNMODE       = 472,
        ERR_INVITEONLYCHAN    = 473,
        ERR_BANNEDFROMCHAN    = 474,
        ERR_BADCHANNELKEY     = 475,
        ERR_BADCHANMASK       = 476,
        ERR_NOCHANMODES       = 477,
        ERR_BANLISTFULL       = 478,
        ERR_NOPRIVILEGES      = 481,
        ERR_CHANOPRIVSNEEDED  = 482,
        ERR_CANTKILLSERVER    = 483,
        ERR_RESTRICTED        = 484,
        ERR_UNIQOPPRIVSNEEDED = 485,
        ERR_NOOPERHOST        = 491,
        ERR_UMODEUNKNOWNFLAG  = 501,
        ERR_USERSDONTMATCH    = 502
    };

	static std::string toString(Code code) {
		switch (code) {
			// Welcome
			case RPL_WELCOME:           return ":RPL_WELCOME";
			case RPL_YOURHOST:          return ":RPL_YOURHOST";
			case RPL_CREATED:           return ":RPL_CREATED";
			case RPL_MYINFO:            return ":RPL_MYINFO";
			case RPL_ISUPPORT:          return ":RPL_ISUPPORT";
			// Server
			case RPL_TRACELINK:         return ":RPL_TRACELINK";
			case RPL_TRACECONNECTING:   return ":RPL_TRACECONNECTING";
			case RPL_TRACEHANDSHAKE:    return ":RPL_TRACEHANDSHAKE";
			case RPL_TRACEUNKNOWN:      return ":RPL_TRACEUNKNOWN";
			case RPL_TRACEOPERATOR:     return ":RPL_TRACEOPERATOR";
			case RPL_TRACEUSER:         return ":RPL_TRACEUSER";
			case RPL_TRACESERVER:       return ":RPL_TRACESERVER";
			case RPL_TRACESERVICE:      return ":RPL_TRACESERVICE";
			case RPL_TRACENEWTYPE:      return ":RPL_TRACENEWTYPE";
			case RPL_TRACECLASS:        return ":RPL_TRACECLASS";
			case RPL_TRACERECONNECT:    return ":RPL_TRACERECONNECT";
			case RPL_STATSLINKINFO:     return ":RPL_STATSLINKINFO";
			case RPL_STATSCOMMANDS:     return ":RPL_STATSCOMMANDS";
			case RPL_STATSCLINE:        return ":RPL_STATSCLINE";
			case RPL_STATSNLINE:        return ":RPL_STATSNLINE";
			case RPL_STATSILINE:        return ":RPL_STATSILINE";
			case RPL_STATSKLINE:        return ":RPL_STATSKLINE";
			case RPL_STATSQLINE:        return ":RPL_STATSQLINE";
			case RPL_STATSYLINE:        return ":RPL_STATSYLINE";
			case RPL_ENDOFSTATS:        return ":RPL_ENDOFSTATS";
			case RPL_UMODEIS:           return ":RPL_UMODEIS";
			case RPL_SERVICEINFO:       return ":RPL_SERVICEINFO";
			case RPL_ENDOFSERVICES:     return ":RPL_ENDOFSERVICES";
			case RPL_SERVICE:           return ":RPL_SERVICE";
			case RPL_SERVLIST:          return ":RPL_SERVLIST";
			case RPL_SERVLISTEND:       return ":RPL_SERVLISTEND";
			case RPL_STATSVLINE:        return ":RPL_STATSVLINE";
			case RPL_STATSLLINE:        return ":RPL_STATSLLINE";
			case RPL_STATSUPTIME:       return ":RPL_STATSUPTIME";
			case RPL_STATSOLINE:        return ":RPL_STATSOLINE";
			case RPL_STATSHLINE:        return ":RPL_STATSHLINE";
			case RPL_STATSSLINE:        return ":RPL_STATSSLINE";
			case RPL_STATSPING:         return ":RPL_STATSPING";
			case RPL_STATSBLINE:        return ":RPL_STATSBLINE";
			case RPL_STATSDLINE:        return ":RPL_STATSDLINE";
			case RPL_LUSERCLIENT:       return ":RPL_LUSERCLIENT";
			case RPL_LUSEROP:           return ":RPL_LUSEROP";
			case RPL_LUSERUNKNOWN:      return ":RPL_LUSERUNKNOWN";
			case RPL_LUSERCHANNELS:     return ":RPL_LUSERCHANNELS";
			case RPL_LUSERME:           return ":RPL_LUSERME";
			case RPL_ADMINME:           return ":RPL_ADMINME";
			case RPL_ADMINLOC1:         return ":RPL_ADMINLOC1";
			case RPL_ADMINLOC2:         return ":RPL_ADMINLOC2";
			case RPL_ADMINEMAIL:        return ":RPL_ADMINEMAIL";
			case RPL_TRACELOG:          return ":RPL_TRACELOG";
			case RPL_TRACEEND:          return ":RPL_TRACEEND";
			case RPL_TRYAGAIN:          return ":RPL_TRYAGAIN";
			// Channels / Users
			case RPL_NONE:              return ":RPL_NONE";
			case RPL_AWAY:              return ":RPL_AWAY";
			case RPL_USERHOST:          return ":RPL_USERHOST";
			case RPL_ISON:              return ":RPL_ISON";
			case RPL_UNAWAY:            return ":RPL_UNAWAY";
			case RPL_NOWAWAY:           return ":RPL_NOWAWAY";
			case RPL_WHOISUSER:         return ":RPL_WHOISUSER";
			case RPL_WHOISSERVER:       return ":RPL_WHOISSERVER";
			case RPL_WHOISOPERATOR:     return ":RPL_WHOISOPERATOR";
			case RPL_WHOWASUSER:        return ":RPL_WHOWASUSER";
			case RPL_ENDOFWHO:          return ":RPL_ENDOFWHO";
			case RPL_WHOISCHANOP:       return ":RPL_WHOISCHANOP";
			case RPL_WHOISIDLE:         return ":RPL_WHOISIDLE";
			case RPL_ENDOFWHOIS:        return ":RPL_ENDOFWHOIS";
			case RPL_WHOISCHANNELS:     return ":RPL_WHOISCHANNELS";
			case RPL_LISTSTART:         return ":RPL_LISTSTART";
			case RPL_LIST:              return ":RPL_LIST";
			case RPL_LISTEND:           return ":RPL_LISTEND";
			case RPL_CHANNELMODEIS:     return ":RPL_CHANNELMODEIS";
			case RPL_UNIQOPIS:          return ":RPL_UNIQOPIS";
			case RPL_NOTOPIC:           return ":RPL_NOTOPIC";
			case RPL_TOPIC:             return ":RPL_TOPIC";
			case RPL_INVITING:          return ":RPL_INVITING";
			case RPL_SUMMONING:         return ":RPL_SUMMONING";
			case RPL_INVITELIST:        return ":RPL_INVITELIST";
			case RPL_ENDOFINVITELIST:   return ":RPL_ENDOFINVITELIST";
			case RPL_EXCEPTLIST:        return ":RPL_EXCEPTLIST";
			case RPL_ENDOFEXCEPTLIST:   return ":RPL_ENDOFEXCEPTLIST";
			case RPL_VERSION:           return ":RPL_VERSION";
			case RPL_WHOREPLY:          return ":RPL_WHOREPLY";
			case RPL_NAMREPLY:          return ":RPL_NAMREPLY";
			case RPL_KILLDONE:          return ":RPL_KILLDONE";
			case RPL_CLOSING:           return ":RPL_CLOSING";
			case RPL_CLOSEEND:          return ":RPL_CLOSEEND";
			case RPL_LINKS:             return ":RPL_LINKS";
			case RPL_ENDOFLINKS:        return ":RPL_ENDOFLINKS";
			case RPL_ENDOFNAMES:        return ":RPL_ENDOFNAMES";
			case RPL_BANLIST:           return ":RPL_BANLIST";
			case RPL_ENDOFBANLIST:      return ":RPL_ENDOFBANLIST";
			case RPL_ENDOFWHOWAS:       return ":RPL_ENDOFWHOWAS";
			case RPL_INFO:              return ":RPL_INFO";
			case RPL_MOTD:              return ":RPL_MOTD";
			case RPL_INFOSTART:         return ":RPL_INFOSTART";
			case RPL_ENDOFINFO:         return ":RPL_ENDOFINFO";
			case RPL_MOTDSTART:         return ":RPL_MOTDSTART";
			case RPL_ENDOFMOTD:         return ":RPL_ENDOFMOTD";
			case RPL_YOUREOPER:         return ":RPL_YOUREOPER";
			case RPL_REHASHING:         return ":RPL_REHASHING";
			case RPL_YOURESERVICE:      return ":RPL_YOURESERVICE";
			case RPL_MYPORTIS:          return ":RPL_MYPORTIS";
			case RPL_NOTOPERANYMORE:    return ":RPL_NOTOPERANYMORE";
			case RPL_TIME:              return ":RPL_TIME";
			case RPL_USERSSTART:        return ":RPL_USERSSTART";
			case RPL_USERS:             return ":RPL_USERS";
			case RPL_ENDOFUSERS:        return ":RPL_ENDOFUSERS";
			case RPL_NOUSERS:           return ":RPL_NOUSERS";
			// Errors
			case ERR_NOSUCHNICK:        return " :No such nick\r\n";
			case ERR_NOSUCHSERVER:      return ":ERR_NOSUCHSERVER";
			case ERR_NOSUCHCHANNEL:     return " :No such channel\r\n";
			case ERR_CANNOTSENDTOCHAN:  return ":ERR_CANNOTSENDTOCHAN";
			case ERR_TOOMANYCHANNELS:   return ":ERR_TOOMANYCHANNELS";
			case ERR_WASNOSUCHNICK:     return ":ERR_WASNOSUCHNICK";
			case ERR_TOOMANYTARGETS:    return ":ERR_TOOMANYTARGETS";
			case ERR_NOSUCHSERVICE:     return ":ERR_NOSUCHSERVICE";
			case ERR_NOORIGIN:          return ":ERR_NOORIGIN";
			case ERR_NORECIPIENT:       return " :No text to send\r\n";
			case ERR_NOTEXTTOSEND:      return ":ERR_NOTEXTTOSEND";
			case ERR_NOTOPLEVEL:        return ":ERR_NOTOPLEVEL";
			case ERR_WILDTOPLEVEL:      return ":ERR_WILDTOPLEVEL";
			case ERR_BADMASK:           return ":ERR_BADMASK";
			case ERR_UNKNOWNCOMMAND:    return ":ERR_UNKNOWNCOMMAND";
			case ERR_NOMOTD:            return ":ERR_NOMOTD";
			case ERR_NOADMININFO:       return ":ERR_NOADMININFO";
			case ERR_FILEERROR:         return ":ERR_FILEERROR";
			case ERR_NONICKNAMEGIVEN:   return ":ERR_NONICKNAMEGIVEN";
			case ERR_ERRONEUSNICKNAME:  return ":ERR_ERRONEUSNICKNAME";
			case ERR_NICKNAMEINUSE:     return ":ERR_NICKNAMEINUSE";
			case ERR_NICKCOLLISION:     return ":ERR_NICKCOLLISION";
			case ERR_UNAVAILRESOURCE:   return ":ERR_UNAVAILRESOURCE";
			case ERR_USERNOTINCHANNEL:  return " :User not on that channel\r\n";
			case ERR_NOTONCHANNEL:      return " :You're not on that channel\r\n";
			case ERR_USERONCHANNEL:     return " :You are already a member\r\n";
			case ERR_NOLOGIN:           return ":ERR_NOLOGIN";
			case ERR_SUMMONDISABLED:    return ":ERR_SUMMONDISABLED";
			case ERR_USERSDISABLED:     return ":ERR_USERSDISABLED";
			case ERR_NOTREGISTERED:     return ":ERR_NOTREGISTERED";
			case ERR_NEEDMOREPARAMS:    return " :Not enough parameters\r\n";
			case ERR_ALREADYREGISTRED:  return ":ERR_ALREADYREGISTRED";
			case ERR_NOPERMFORHOST:     return ":ERR_NOPERMFORHOST";
			case ERR_PASSWDMISMATCH:    return ":ERR_PASSWDMISMATCH";
			case ERR_YOUREBANNEDCREEP:  return ":ERR_YOUREBANNEDCREEP";
			case ERR_YOUWILLBEBANNED:   return ":ERR_YOUWILLBEBANNED";
			case ERR_KEYSET:            return ":ERR_KEYSET";
			case ERR_CHANNELISFULL:     return " :Channel is full\r\n";
			case ERR_UNKNOWNMODE:       return ":ERR_UNKNOWNMODE";
			case ERR_INVITEONLYCHAN:    return " :Invite only channel\r\n";
			case ERR_BANNEDFROMCHAN:    return ":ERR_BANNEDFROMCHAN";
			case ERR_BADCHANNELKEY:     return " :Bad channel key\r\n";
			case ERR_BADCHANMASK:       return " :Bad Channel Mask\r\n";
			case ERR_NOCHANMODES:       return ":ERR_NOCHANMODES";
			case ERR_BANLISTFULL:       return ":ERR_BANLISTFULL";
			case ERR_NOPRIVILEGES:      return ":ERR_NOPRIVILEGES";
			case ERR_CHANOPRIVSNEEDED:  return " :You're not channel operator\r\n";
			case ERR_CANTKILLSERVER:    return ":ERR_CANTKILLSERVER";
			case ERR_RESTRICTED:        return ":ERR_RESTRICTED";
			case ERR_UNIQOPPRIVSNEEDED: return ":ERR_UNIQOPPRIVSNEEDED";
			case ERR_NOOPERHOST:        return ":ERR_NOOPERHOST";
			case ERR_UMODEUNKNOWNFLAG:  return ":ERR_UMODEUNKNOWNFLAG";
			case ERR_USERSDONTMATCH:    return ":ERR_USERSDONTMATCH";
			default:                    return ":UNKNOWN";
		}
	}
};
