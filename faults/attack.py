
import random
from math import log2, ceil
from ChaCha import ChaCha
from bytes_utils import xor, chunkbytes
from blake import BLAKE

def toByteArray(string): return bytes(string, "latin-1")

def toHexString(bytes): return ''.join('{:02x}'.format(x) for x in bytes)

in_sk = "Cé qu'è lainô, le Maitre dé bataille, Que se moqué et se ri dé c"
in_masks = "anaille, A bin fai vi, pè on desande nai, Qu'il étivé patron dé Genevoi. I son vegnu le doze de dessanbro, Pè onna nai asse naire que d'ancro; Y étivé l'an mil si san et dou, Qu'i veniron parla ou pou troi tou. Pè onna nai qu'étive la pe naire, I veniron; y n'étai pas pè bairè: Y étivé pè pilli nou maison, Et no tüa sans aucuna raison Petis et grans, ossis an sevegnance: Pè on matin d'onna bella demanze, Et pè on zeur qu'y fassive bin frai, Sans le bon Di, nos étivon to prai! On vo dera qu'étai cela canaille. - Lou Savoyar contre noutra mouraille Trai eitiellé on dressia et plianta, Et par iqué dou san y son monta. Etian antra, veniron u courdegarda, Yo i firon onna ruda montada. Is avion tenaillé et marté Qu'étivon fai avoi du boun acié, Pè arraci lou cliou et lé saraille, To lou verreu et tota la féraille Qu'on rancontré an dé pari andrai, Et qu'on bouté pè n'eitre pas surprai. On eitablio is avivon forcia; Et d'on petar qu'is avivon teria, I coudavon deiza eitre à sevau: I n'étivon pas assé monta yo. Sen Altessé dessu Pincha étive. Yon d'antre leu s'ancoru pè li dire Q"

in_sk1_bytes   = toByteArray(in_sk[:32])
in_sk2_bytes   = toByteArray(in_sk[32:])
in_masks_bytes = toByteArray(in_masks)

exp_root  = bytearray.fromhex("0b87b5678179f3c3614ce6181380ee0b434eb7f2aefb9af73e2bcd379b341c95")
exp_sigma = chunkbytes(bytearray.fromhex("309ae7ee4abbf3f14f25c3973b62579f176f2cdcb349fa4e32097452bdc2019508974e0c32e99d7c6c6929f099df9052b3fd5c54a8f63e15e06683234fa4014b56e56b21fc94d88ee98cf36aafc81ef9f05201fb755df9b20fb3f7b0f80c258c87dd7bc2927a07617188b3f2d698135763c3dd0667f1291039879fe1c71132b7ec4179296a2619d1b3c01942d45be8f5e792fcf2fb4d16328f3bcdb5f9103c8557e68fcdb234f578956429c39ad0f98092d2f5f885dd9eb8c4e3931474b736ca751d3d247d28506e24de2758cf524b80eb75a6e168426fb38761317fe47c9b302810598c6bc44da3150d8f992d81c0c38458bc613d76b0cda86fdbb7a2b4e89374770064092e719bb735775364231055e62f431361f10b93883e91007c5e43d0b924839590f92feeac6be7852b2f6934f262bee5749574b31194d946f00d9495b27df035c833192a5d0eac8fa80eb8da3f4c95558fcbc155fef2cae7847450bb266775bc3b83cd9064e712697ed6813ca82589d5128d1c8e2786aef4d1a2898e58e9f7ec406829e8fdc35f5b2854a7ee96bd406c9c89b580f941396aa1af1605b87be8040a3832b734f9293ba9d30131d04b0be40931cb3cbaf0be6470d35bd825ab05bb8c67a3751f516e47e9e0354204310a1b1cc0d45f574313de31c0c0993c799aa28dab860045544cfe5f5d860e454b2eb1f927381ba6265582573b1091ebf2f71cd2bd027d41ae6393be3845ad7101a1c2112edac7adeea51518fb68a5465639421503a3a0f26cad6424e3a7dbfb99563fc5d4cd2bc25158b7a85d0265ad28ef1785eb5e128c10105b1c0f64797bd3dcbbb1066eb01bc3fa2be6b629dfefa4947dceaa0f8e0ab1e1afab88c2f9264b1fdf154d098d7aab62b8da8594ef0774b713f49e826b4835bc1256b04628c7af239932255bc800979b9eb7dff05f3b4d1223fbafe4e4c370f86d1155a02add33ce0d56cb99c047076bd535219bbd92f6f9ba8d25a81b65917f5880b2f3a06bd521c20d4fbb38873202914fff360b09626e1f04e737dab3c5a1ac1e2de0656536e418aaca8cf12effbf241c0fa8778ce8c32f5466fd163b38d71c7995b380567fb11bd4e2e6cba287195b1f7351bcf442977a44fdfeaf92b44b240bc28fb2e1863a1f0ad24274f0f251cbbf7cdd10cad89bf7f4305dabfffccb79ab9f7318bb258c70dfb4867063227acfaf921a931081359f7889cf960e26077d8a0210fc4f4b2bab4b48fa081ec00dd71f588a5f477d28c2e186dadabb7fca45b2a8b97e1e4ebbc370527ac6c4ffcc02219d7a04871dbb10c7cc2d054f1c720ed13ad296174d9771139a4dbb5826589de11f27664dd535771a9738bd3549ec5b077756e66d59fb7ec14a358ebee661d6625a718037e790d28442accaff40ce98f45d2c74516888d07c7f46f412bea13919e0cf4936745a9486f3ccc91680d89c8f22f55b6e33d24bf749a78e14ed2188086fd800773f8ab3ae9b938f7c8e24f9e9c2f003ce9f5a4ab2c3ef35eb7b072eb594788ed34dfc1cc08f77344b01c5291c9e114950963b2657878e5da9fbd979cc1646ab4be1c3b5f62260d4f0cc2e85da9563ee12fd6a437e6a0f2f63899e2586023e5f76d4963e4988ae47f6684dca4df688edece725d608241622202169c9875f470dde8cf96142e3b91e21d4ad0f70716ad53bc739f3cbd089c1b5e70a87754f0648a98715afb14e62d142935c61819018be87a510c25fd95de566adc64022da0bd4cee31ece54884f2302c77bfa1a1c7cb464a458f61a76bb2e4178a48e09d85c0f155b32fc4e3df27c19d43ca0737d1c9e03eca3f8d206bdcb39ac64600fb96fcb90d304778d36169127e9f0386e87f56f252519ca643fc5caeae0f27a537947f24e648fcdf76a0273a0041a13df8e3fd0d203dc73845bf0f2ae6ac3ce9e1af589857e7a3b0ce037af829e6358c7c45c4982f14ccf51a534937dffe3b6338bb86172e4761f4523fd4bfe18b94cf9954fcc049ecab08b4d40c25134c0dd4890fc456f8f4fec9f77c106265992a7feaf506fa7d4a78264ea44f90cfd0c333b1f149a27e1ad5ddf0ca77c7f1a2c9c3664afa95b6bc4ee067adf6d7d8083d70e8583eb2cf177fde4c49f03ce4304ac0f8c652ac0f99728a454ce20a3c6517556d88650e6109085c1f09fc271e381adb1ba36fd0f09adfd3268c0e6e14060e2a3cea38bd658dff684c4f613552cfc63374ae253398c40f6f3763935aa019d9ac36e10110a7014330ee12cf2a852f42f6f5ba2090201075ed04bcd41ffc3fcbbe2ab5591104f6d27ca8a28831459f69bbe9c65a626ba6acc42cdd94323a88411c573d9795ff4adfb378774930c9aad3fcccb398dda73d967a767d610cd2318776955807bc86879db091171098c6fa028b0deddcfa4cb63a79e94d1bfb26047c7fcfa526bc04b1fb3c0792af1df3cd32d730d91fbf84e504393013bd44910498988f0acff6593c3cc9814e384132c4e6c59950e316fc9cf6843938e5276eb490c5eb7b4a770dd54461682a6269c1d11575186f8b49657af2c3935c4912540e78eb092aee7bc6b107330f8d258912846badaaca0fb2d4e1684a197b821877bd19378101b3db0b356bdfd5ddebfa61694166f0fea6ce981ca3925a5f7f7664dc60fedd6092aa76cc9b648678ea969d769674c5be84c64b63a9196ec42930b17a9940c0008a24c3fd91d4dd6ee61bbb7191479af1e880bae3abc0385a8794178ceb857d5c65b2e8a1d9253ee5e089fef571f2eb8b8185439c84a155c7c0f80f34442bc5f649c89ee4af4c1341342478dfba3ecee02e2c1ce5cdc8fcbb88164280a8d4b1f09bedb4a1e584a31a958961728726f7a181a3cf79df399ae693c51c47fa270eabf1324f38dafd40935b3fb93331da00265eb367a51824b7894b0ddfe5b858c7eebdd52f390f0db6c2769c333110235dd059889131f371a5661e17ab30faefb82fe0ebe0ee3308d3905e2c3dfe11a28225f8b9972b9010f934bde55c6453523bf5bbf"), 32)
subtree_A = int("07c1d4f115363236", 16)
target_A  = int("103e0ea788a9b191", 16)

Glambda = lambda seed, n: ChaCha(key=seed).keystream(n)
Gl = lambda seed: Glambda(seed=seed, n=67*32)

perm = ChaCha().permuted
F = lambda m: perm(m + toByteArray("expand 32-byte to 64-byte state!"))[:32]
H = lambda m1, m2: perm(xor(perm(m1 + toByteArray("expand 32-byte to 64-byte state!")), m2 + bytes(32)))[:32]

Fa = lambda a, k: BLAKE(256).digest(k + a)
Frand = lambda m, k: BLAKE(512).digest(k + m)

n = 256
l = 64 + 3
sphincs_bytes = 32
w = 16
h = 60
d = 12

def keygen(in_seed, in_masks):
	"""
	W-OTS+ key generation algorithm.
	"""
	sk = chunkbytes(Gl(in_seed), sphincs_bytes)
	masks = chunkbytes(in_masks, sphincs_bytes)

	pk = list(sk)
	for i in range(0, w-1):
		for j in range(len(pk)):
			pk[j] = F(xor(pk[j], masks[i]))

	return pk

def int_to_basew(l, x, base):
	"""
	Block splitting procedure.
	"""
	for _ in range(l):
		yield x % base
		x //= base

def sign(in_digest, in_seed, in_masks):
	"""
	W-OTS+ signing algorithm.
	"""
	M = list(int_to_basew(64, int.from_bytes(in_digest, byteorder="big"), 16))[::-1]
	C = list(int_to_basew(3, sum(15 - M[i] for i in range(len(M))), 16))[::-1]

	B = M + C

	sk = chunkbytes(Gl(in_seed), sphincs_bytes)
	masks = chunkbytes(in_masks, sphincs_bytes)

	sign = list(sk)
	for i in range(len(sk)):
		for j in range(0, B[i]):
			sign[i] = F(xor(sign[i], masks[j]))

	return ''.join([toHexString(s) for s in sign])


def verify(in_digest, in_sigs, in_masks):
	"""
	W-OTS+ verification algorithm.
	"""
	M = list(int_to_basew(64, int.from_bytes(in_digest, byteorder="big"), 16))[::-1]
	C = list(int_to_basew(3, sum(15 - M[i] for i in range(len(M))), 16))[::-1]

	B = M + C

	masks = chunkbytes(in_masks, sphincs_bytes)

	pk = list(in_sigs)
	for i in range(len(pk)):
		for j in range(B[i], w-1):
			pk[i] = F(xor(pk[i], masks[j]))

	return pk

def trysign(in_digest, dico, in_masks):
	"""
	Attempts to produce a W-OTS+ signature with the dictionary of secret values.
	"""
	M = list(int_to_basew(64, int.from_bytes(in_digest, byteorder="big"), 16))[::-1]
	C = list(int_to_basew(3, sum(15 - M[i] for i in range(len(M))), 16))[::-1]

	B = M + C

	masks = chunkbytes(in_masks, sphincs_bytes)

	sig = []
	for i in range(len(B)):
		if dico[i][0] <= B[i]:
			sig += [dico[i][1]]
			for j in range(dico[i][0], B[i]):
				sig[i] = F(xor(sig[i], masks[j]))
		else:
			return False
	return sig

def l_tree(H, leafs):
	"""
	L-Tree construction algorithm.
	"""
	layer = leafs
	yield layer
	for i in range(ceil(log2(len(leafs)))):
		next_layer = [H(l, r, i) for l, r in zip(layer[0::2], layer[1::2])]
		if len(layer) & 1:
			next_layer.append(layer[-1])
		layer = next_layer
		yield layer

def root(tree):
	"""
	Returns the root of a the l_tree construction algorithm.
	"""
	return list(tree)[-1][0]

def wots_leaf(in_H, in_addr, in_sk1, in_masks):
	"""
	Discovers a new W-OTS+ leaf in subtree at address in_addr.
	"""

	seed = Fa(in_addr, in_sk1)
	wots_pk = keygen(seed, in_masks)

	return root(l_tree(in_H, wots_pk))

def attack(faulty_file, sigs_amount, seed=0):
	"""
	Attacks CMSS with faulty signatures.
	"""

	faulty_sigs = [bytearray.fromhex(line.rstrip('\n')) for line in open(faulty_file)]
	wots_pk = verify(exp_root, exp_sigma, in_masks_bytes)

	faulty_sigs = [chunkbytes(fsig, 32) for fsig in faulty_sigs[:sigs_amount]]
	faulty_sigs += [exp_sigma]

	masks = chunkbytes(in_masks_bytes, 32)

	dico = dict()
	for i in range(len(wots_pk)):
		dico[i] = (w - 1, wots_pk[i])

	for sig in faulty_sigs:
		for s in range(len(sig)):
			for i in range(w - 1):
				x = sig[s]
				for j in range(w - 1 - i):
					x = F(xor(x, masks[i + j]))
					if x == wots_pk[s]:
						if dico[s][0] > i:
							dico[s] = (i, sig[s])

	count = 0
	sigma = False
	random.seed(seed)
	address = subtree_A
	xorH = lambda x, y, i: H(xor(x, masks[2*i]), xor(y, masks[2*i+1]))

	while not sigma:
		sk1 = bytearray((random.getrandbits(8) for i in range(n // 8)))
		leaves = []
		for j in range((1 << h // d)):
			address = (address & 0xfffffffffffffff0) | j
			leaves += [wots_leaf(xorH, int.to_bytes(address, length=8, byteorder='big'), sk1, in_masks_bytes)]
		tree = l_tree(xorH, leaves)
		sigma = trysign(root(tree), dico, in_masks_bytes)
		count += 1

	print("Amount: " + str(count))
	print(''.join([toHexString(s) for s in sigma]))
	print("sk_1: " + toHexString(sk1))
	return dico
