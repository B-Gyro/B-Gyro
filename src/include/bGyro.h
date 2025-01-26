# pragma once

# include "klibc/types.h"

typedef enum bGyroStatus {
    B_GYRO_STABLE,
    B_GYRO_ERROR,
    B_GYRO_UNKNOWN,
} e_bGyroStatus;

typedef struct bGyroStats {
	char			*OSVersion;
    e_bGyroStatus	status;
    bool			isPaginated;
    size_t			mainEBP;
	bool			hasSerialWorking;
	bool			isTTYMode;
} _bGyroStats;


void	bGyroSetStat(e_bGyroStatus bGStatus);
char	*bGyroStatusToString(e_bGyroStatus status);