/*
 * parameters.h
 *
 */

/* SPHINCS parameters */
#define SPHINCS_N 256
#define SPHINCS_BYTES 32 /* (SPHINCS_N/8) */
#define SPHINCS_DIGEST_BYTES 64 /* (2*SPHINCS_BYTES) */

/* Key sizes */
#define MASKS_AMOUNT 32 /* max(2*(MSS_TREE_HEIGHT + WOTS_LOG_L),
                               WOTS_MAX_INT - 1,
                               2*log2(HORST_T))
                           = 2*log2(HORST_T) */
#define MASKS_BYTES 1024 /* (MASKS_AMOUNT*SPHINCS_BYTES) */
#define SK_BYTES 1088 /* (2*SPHINCS_BYTES) + MASKS_BYTES */
#define PK_BYTES 1056 /* (1*SPHINCS_BYTES) + MASKS_BYTES */

/* Merkle's trees parameters and quantities */
#define MSS_TREE_HEIGHT 5
#define MSS_LEVELS 12
#define MSS_TOTALTREE_HEIGHT 60 /* (MSS_TREE_HEIGHT*MSS_LEVELS) */

/* PRNG parameters */
#define SEED_BYTES 32 /* SPHINCS_BYTES */

/* HORST parameters and quantities */
#define HORST_T (1 << 16) /* 2^16 */
#define HORST_K 32
#define HORST_SK_BYTES 32
#define HORST_SIG_BYTES 13312 /* (64*SPHINCS_BYTES + (((log2(HORS_T) - 6)*SPHINCS_BYTES)
                                  + HORST_SK_BYTES)*HORST_K) */

/* WOTS parameters and quantities */
#define WOTS_W 4
#define WOTS_MAX_INT 16 /* (1 << WOTS_W) */
#define WOTS_L1 64 /* ceil(SPHINCS_SECURITY_PARAMETER/WOTS_W) */
#define WOTS_L2 3 /* ceil(log2(WOTS_L1*(WOTS_MAX_INT-1))/WOTS_W) */
#define WOTS_L 67 /* WOTS_L1 + WOTS_L2 */
#define WOTS_LOG_L 7 /* log2(WOTS_L) */
#define WOTS_SIG_BYTES 2144 /* (SPHINCS_BYTES*WOTS_L) */
