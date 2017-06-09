/*
 * horst.h
 *
 */

#include "parameters.h"

#ifndef HORST_H_
#define HORST_H_

/*
 * Prints HORST public key on stdout associated with the provided seed and
 * masks.
 *
 * The key generation uses the treehash algorithm to compute the root of the
 * HORST tree.
 */

int horst_keygen(unsigned char const seed[SEED_BYTES],
                 unsigned char const masks[HORST_TAU*SPHINCS_BYTES]);

/*
 * Prints textbook HORST signature of a digest on stdout with respect to
 * provided seed and masks.
 *
 * The HORST signature consists of revealing secret values along with their own
 * authentication path, so a verifier can reconstruct the HORST tree.
 *
 * Because of memory limitations, the procedure makes as many treehashes as
 * required to compute authentication path of all secret values.
 */

int horst_sign(unsigned char const digest[SPHINCS_DIGEST_BYTES],
               unsigned char const seed[SEED_BYTES],
               unsigned char const masks[HORST_TAU*SPHINCS_BYTES]);

/*
 * Prints optimized HORST signature of a digest on stdout with respect to
 * provided seed and masks, as it is done in the SPHINCS cryptosystem.
 *
 * In this variant, the signature is made out of order. The sixth layer is
 * always printed out, so the authentication paths can be truncated after the
 * tenth node.
 *
 * The procedure only makes one treehash call, during which all the encountered
 * nodes which belong to the signature are printed out.
 */

int horst_sign_opti(unsigned char const digest[SPHINCS_DIGEST_BYTES],
                    unsigned char const seed[SEED_BYTES],
                    unsigned char const masks[HORST_TAU*SPHINCS_BYTES]);

/*
 * Verifies the validity of a HORST signature for a digest under the provided
 * public key and masks.
 *
 * The signature is assumed to be available in the sig_stream function. It is
 * also assumed that the nodes come in the same order as the horst_sign_opti,
 * which corresponds to a treehash order.
 *
 * The procedure runs a re-arranged variant of the treehash algorithm to
 * compute the top layer (HORST_MAX_LEVEL). This variant reads nodes from the
 * signature stream, checks that secret value ids correspond to message digest,
 * stores top layer nodes to compare them to the received top layer nodes and
 * to eventually construct the tree root, so it can be compared to the public
 * key.
 */

int horst_verify_opti(unsigned char const digest[SPHINCS_DIGEST_BYTES],
                      unsigned char const y[SPHINCS_BYTES],
                      int (*sig_stream)(void),
                      unsigned char const masks[HORST_TAU*SPHINCS_BYTES]);

#endif /* HORST_H_ */
