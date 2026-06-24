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
			case RPL_WELCOME:           return ":RPL_WELCOME\r\n";
			case RPL_YOURHOST:          return ":RPL_YOURHOST\r\n";
			case RPL_CREATED:           return ":RPL_CREATED\r\n";
			case RPL_MYINFO:            return ":RPL_MYINFO\r\n";
			case RPL_ISUPPORT:          return ":RPL_ISUPPORT\r\n";
			// Serv\r\ner
			case RPL_TRACELINK:         return ":RPL_TRACELINK\r\n";
			case RPL_TRACECONNECTING:   return ":RPL_TRACECONNECTING\r\n";
			case RPL_TRACEHANDSHAKE:    return ":RPL_TRACEHANDSHAKE\r\n";
			case RPL_TRACEUNKNOWN:      return ":RPL_TRACEUNKNOWN\r\n";
			case RPL_TRACEOPERATOR:     return ":RPL_TRACEOPERATOR\r\n";
			case RPL_TRACEUSER:         return ":RPL_TRACEUSER\r\n";
			case RPL_TRACESERVER:       return ":RPL_TRACESERVER\r\n";
			case RPL_TRACESERVICE:      return ":RPL_TRACESERVICE\r\n";
			case RPL_TRACENEWTYPE:      return ":RPL_TRACENEWTYPE\r\n";
			case RPL_TRACECLASS:        return ":RPL_TRACECLASS\r\n";
			case RPL_TRACERECONNECT:    return ":RPL_TRACERECONNECT\r\n";
			case RPL_STATSLINKINFO:     return ":RPL_STATSLINKINFO\r\n";
			case RPL_STATSCOMMANDS:     return ":RPL_STATSCOMMANDS\r\n";
			case RPL_STATSCLINE:        return ":RPL_STATSCLINE\r\n";
			case RPL_STATSNLINE:        return ":RPL_STATSNLINE\r\n";
			case RPL_STATSILINE:        return ":RPL_STATSILINE\r\n";
			case RPL_STATSKLINE:        return ":RPL_STATSKLINE\r\n";
			case RPL_STATSQLINE:        return ":RPL_STATSQLINE\r\n";
			case RPL_STATSYLINE:        return ":RPL_STATSYLINE\r\n";
			case RPL_ENDOFSTATS:        return ":RPL_ENDOFSTATS\r\n";
			case RPL_UMODEIS:           return ":RPL_UMODEIS\r\n";
			case RPL_SERVICEINFO:       return ":RPL_SERVICEINFO\r\n";
			case RPL_ENDOFSERVICES:     return ":RPL_ENDOFSERVICES\r\n";
			case RPL_SERVICE:           return ":RPL_SERVICE\r\n";
			case RPL_SERVLIST:          return ":RPL_SERVLIST\r\n";
			case RPL_SERVLISTEND:       return ":RPL_SERVLISTEND\r\n";
			case RPL_STATSVLINE:        return ":RPL_STATSVLINE\r\n";
			case RPL_STATSLLINE:        return ":RPL_STATSLLINE\r\n";
			case RPL_STATSUPTIME:       return ":RPL_STATSUPTIME\r\n";
			case RPL_STATSOLINE:        return ":RPL_STATSOLINE\r\n";
			case RPL_STATSHLINE:        return ":RPL_STATSHLINE\r\n";
			case RPL_STATSSLINE:        return ":RPL_STATSSLINE\r\n";
			case RPL_STATSPING:         return ":RPL_STATSPING\r\n";
			case RPL_STATSBLINE:        return ":RPL_STATSBLINE\r\n";
			case RPL_STATSDLINE:        return ":RPL_STATSDLINE\r\n";
			case RPL_LUSERCLIENT:       return ":RPL_LUSERCLIENT\r\n";
			case RPL_LUSEROP:           return ":RPL_LUSEROP\r\n";
			case RPL_LUSERUNKNOWN:      return ":RPL_LUSERUNKNOWN\r\n";
			case RPL_LUSERCHANNELS:     return ":RPL_LUSERCHANNELS\r\n";
			case RPL_LUSERME:           return ":RPL_LUSERME\r\n";
			case RPL_ADMINME:           return ":RPL_ADMINME\r\n";
			case RPL_ADMINLOC1:         return ":RPL_ADMINLOC1\r\n";
			case RPL_ADMINLOC2:         return ":RPL_ADMINLOC2\r\n";
			case RPL_ADMINEMAIL:        return ":RPL_ADMINEMAIL\r\n";
			case RPL_TRACELOG:          return ":RPL_TRACELOG\r\n";
			case RPL_TRACEEND:          return ":RPL_TRACEEND\r\n";
			case RPL_TRYAGAIN:          return ":RPL_TRYAGAIN\r\n";
			// Channels / Users
			case RPL_NONE:              return ":RPL_NONE\r\n";
			case RPL_AWAY:              return ":RPL_AWAY\r\n";
			case RPL_USERHOST:          return ":RPL_USERHOST\r\n";
			case RPL_ISON:              return ":RPL_ISON\r\n";
			case RPL_UNAWAY:            return ":RPL_UNAWAY\r\n";
			case RPL_NOWAWAY:           return ":RPL_NOWAWAY\r\n";
			case RPL_WHOISUSER:         return ":RPL_WHOISUSER\r\n";
			case RPL_WHOISSERVER:       return ":RPL_WHOISSERVER\r\n";
			case RPL_WHOISOPERATOR:     return ":RPL_WHOISOPERATOR\r\n";
			case RPL_WHOWASUSER:        return ":RPL_WHOWASUSER\r\n";
			case RPL_ENDOFWHO:          return ":RPL_ENDOFWHO\r\n";
			case RPL_WHOISCHANOP:       return ":RPL_WHOISCHANOP\r\n";
			case RPL_WHOISIDLE:         return ":RPL_WHOISIDLE\r\n";
			case RPL_ENDOFWHOIS:        return ":RPL_ENDOFWHOIS\r\n";
			case RPL_WHOISCHANNELS:     return ":RPL_WHOISCHANNELS\r\n";
			case RPL_LISTSTART:         return ":RPL_LISTSTART\r\n";
			case RPL_LIST:              return ":RPL_LIST\r\n";
			case RPL_LISTEND:           return ":RPL_LISTEND\r\n";
			case RPL_CHANNELMODEIS:     return ":RPL_CHANNELMODEIS\r\n";
			case RPL_UNIQOPIS:          return ":RPL_UNIQOPIS\r\n";
			case RPL_NOTOPIC:           return ":RPL_NOTOPIC\r\n";
			case RPL_TOPIC:             return ":RPL_TOPIC\r\n";
			case RPL_INVITING:          return ":RPL_INVITING\r\n";
			case RPL_SUMMONING:         return ":RPL_SUMMONING\r\n";
			case RPL_INVITELIST:        return ":RPL_INVITELIST\r\n";
			case RPL_ENDOFINVITELIST:   return ":RPL_ENDOFINVITELIST\r\n";
			case RPL_EXCEPTLIST:        return ":RPL_EXCEPTLIST\r\n";
			case RPL_ENDOFEXCEPTLIST:   return ":RPL_ENDOFEXCEPTLIST\r\n";
			case RPL_VERSION:           return ":RPL_VERSION\r\n";
			case RPL_WHOREPLY:          return ":RPL_WHOREPLY\r\n";
			case RPL_NAMREPLY:          return ":RPL_NAMREPLY\r\n";
			case RPL_KILLDONE:          return ":RPL_KILLDONE\r\n";
			case RPL_CLOSING:           return ":RPL_CLOSING\r\n";
			case RPL_CLOSEEND:          return ":RPL_CLOSEEND\r\n";
			case RPL_LINKS:             return ":RPL_LINKS\r\n";
			case RPL_ENDOFLINKS:        return ":RPL_ENDOFLINKS\r\n";
			case RPL_ENDOFNAMES:        return ":RPL_ENDOFNAMES\r\n";
			case RPL_BANLIST:           return ":RPL_BANLIST\r\n";
			case RPL_ENDOFBANLIST:      return ":RPL_ENDOFBANLIST\r\n";
			case RPL_ENDOFWHOWAS:       return ":RPL_ENDOFWHOWAS\r\n";
			case RPL_INFO:              return ":RPL_INFO\r\n";
			case RPL_MOTD:              return ":RPL_MOTD\r\n";
			case RPL_INFOSTART:         return ":RPL_INFOSTART\r\n";
			case RPL_ENDOFINFO:         return ":RPL_ENDOFINFO\r\n";
			case RPL_MOTDSTART:         return ":RPL_MOTDSTART\r\n";
			case RPL_ENDOFMOTD:         return ":RPL_ENDOFMOTD\r\n";
			case RPL_YOUREOPER:         return ":RPL_YOUREOPER\r\n";
			case RPL_REHASHING:         return ":RPL_REHASHING\r\n";
			case RPL_YOURESERVICE:      return ":RPL_YOURESERVICE\r\n";
			case RPL_MYPORTIS:          return ":RPL_MYPORTIS\r\n";
			case RPL_NOTOPERANYMORE:    return ":RPL_NOTOPERANYMORE\r\n";
			case RPL_TIME:              return ":RPL_TIME\r\n";
			case RPL_USERSSTART:        return ":RPL_USERSSTART\r\n";
			case RPL_USERS:             return ":RPL_USERS\r\n";
			case RPL_ENDOFUSERS:        return ":RPL_ENDOFUSERS\r\n";
			case RPL_NOUSERS:           return ":RPL_NOUSERS\r\n";
			// Errors
			case ERR_NOSUCHNICK:        return " :No such nick\r\n";
			case ERR_NOSUCHSERVER:      return ":ERR_NOSUCHSERVER\r\n";
			case ERR_NOSUCHCHANNEL:     return " :No such channel\r\n";
			case ERR_CANNOTSENDTOCHAN:  return ":ERR_CANNOTSENDTOCHAN\r\n";
			case ERR_TOOMANYCHANNELS:   return ":ERR_TOOMANYCHANNELS\r\n";
			case ERR_WASNOSUCHNICK:     return ":ERR_WASNOSUCHNICK\r\n";
			case ERR_TOOMANYTARGETS:    return ":ERR_TOOMANYTARGETS\r\n";
			case ERR_NOSUCHSERVICE:     return ":ERR_NOSUCHSERVICE\r\n";
			case ERR_NOORIGIN:          return ":ERR_NOORIGIN\r\n";
			case ERR_NORECIPIENT:       return " :No text to send\r\n";
			case ERR_NOTEXTTOSEND:      return ":ERR_NOTEXTTOSEND\r\n";
			case ERR_NOTOPLEVEL:        return ":ERR_NOTOPLEVEL\r\n";
			case ERR_WILDTOPLEVEL:      return ":ERR_WILDTOPLEVEL\r\n";
			case ERR_BADMASK:           return ":ERR_BADMASK\r\n";
			case ERR_UNKNOWNCOMMAND:    return ":ERR_UNKNOWNCOMMAND\r\n";
			case ERR_NOMOTD:            return ":ERR_NOMOTD\r\n";
			case ERR_NOADMININFO:       return ":ERR_NOADMININFO\r\n";
			case ERR_FILEERROR:         return ":ERR_FILEERROR\r\n";
			case ERR_NONICKNAMEGIVEN:   return ":ERR_NONICKNAMEGIVEN\r\n";
			case ERR_ERRONEUSNICKNAME:  return ":ERR_ERRONEUSNICKNAME\r\n";
			case ERR_NICKNAMEINUSE:     return " :Nickname is already in use\r\n";
			case ERR_NICKCOLLISION:     return ":ERR_NICKCOLLISION\r\n";
			case ERR_UNAVAILRESOURCE:   return ":ERR_UNAVAILRESOURCE\r\n";
			case ERR_USERNOTINCHANNEL:  return " :User not on that channel\r\n";
			case ERR_NOTONCHANNEL:      return " :You're not on that channel\r\n";
			case ERR_USERONCHANNEL:     return " :You are already a member\r\n";
			case ERR_NOLOGIN:           return ":ERR_NOLOGIN\r\n";
			case ERR_SUMMONDISABLED:    return ":ERR_SUMMONDISABLED\r\n";
			case ERR_USERSDISABLED:     return ":ERR_USERSDISABLED\r\n";
			case ERR_NOTREGISTERED:     return ":ERR_NOTREGISTERED\r\n";
			case ERR_NEEDMOREPARAMS:    return " :Not enough parameters\r\n";
			case ERR_ALREADYREGISTRED:  return ":ERR_ALREADYREGISTRED\r\n";
			case ERR_NOPERMFORHOST:     return ":ERR_NOPERMFORHOST\r\n";
			case ERR_PASSWDMISMATCH:    return ":ERR_PASSWDMISMATCH\r\n";
			case ERR_YOUREBANNEDCREEP:  return ":ERR_YOUREBANNEDCREEP\r\n";
			case ERR_YOUWILLBEBANNED:   return ":ERR_YOUWILLBEBANNED\r\n";
			case ERR_KEYSET:            return ":ERR_KEYSET\r\n";
			case ERR_CHANNELISFULL:     return " :Channel is full\r\n";
			case ERR_UNKNOWNMODE:       return ":ERR_UNKNOWNMODE\r\n";
			case ERR_INVITEONLYCHAN:    return " :Invite only channel\r\n";
			case ERR_BANNEDFROMCHAN:    return ":ERR_BANNEDFROMCHAN\r\n";
			case ERR_BADCHANNELKEY:     return " :Bad channel key\r\n";
			case ERR_BADCHANMASK:       return " :Bad Channel Mask\r\n";
			case ERR_NOCHANMODES:       return ":ERR_NOCHANMODES\r\n";
			case ERR_BANLISTFULL:       return ":ERR_BANLISTFULL\r\n";
			case ERR_NOPRIVILEGES:      return ":ERR_NOPRIVILEGES\r\n";
			case ERR_CHANOPRIVSNEEDED:  return " :You're not channel operator\r\n";
			case ERR_CANTKILLSERVER:    return ":ERR_CANTKILLSERVER\r\n";
			case ERR_RESTRICTED:        return ":ERR_RESTRICTED\r\n";
			case ERR_UNIQOPPRIVSNEEDED: return ":ERR_UNIQOPPRIVSNEEDED\r\n";
			case ERR_NOOPERHOST:        return ":ERR_NOOPERHOST\r\n";
			case ERR_UMODEUNKNOWNFLAG:  return ":ERR_UMODEUNKNOWNFLAG\r\n";
			case ERR_USERSDONTMATCH:    return ":ERR_USERSDONTMATCH\r\n";
			default:                    return ":UNKNOWN\r\n";
		}
	}
};
