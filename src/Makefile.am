DIST_SUBDIRS = secp256k1 univalue
SUBDIRS = cryptoconditions
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(SAN_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(SAN_CXXFLAGS) $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	$(AM_V_at)$(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

BITCOIN_CONFIG_INCLUDES=-I$(builddir)/config
BITCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

BITCOIN_INCLUDES += -I$(srcdir)/secp256k1/include
BITCOIN_INCLUDES += -I$(srcdir)/cryptoconditions/include
BITCOIN_INCLUDES += -I$(srcdir)/cryptoconditions/src
BITCOIN_INCLUDES += -I$(srcdir)/cryptoconditions/src/asn
BITCOIN_INCLUDES += -I$(srcdir)/univalue/include

LIBBITCOIN_SERVER=libbitcoin_server.a
LIBBITCOIN_WALLET=libbitcoin_wallet.a
LIBBITCOIN_COMMON=libbitcoin_common.a
LIBBITCOIN_CLI=libbitcoin_cli.a
LIBBITCOIN_UTIL=libbitcoin_util.a
LIBBITCOIN_CRYPTO=crypto/libbitcoin_crypto.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBCRYPTOCONDITIONS=cryptoconditions/libcryptoconditions_core.a
LIBUNIVALUE=univalue/libunivalue.la
LIBCC=libcc.a
LIBZCASH=libzcash.a

if ENABLE_ZMQ
LIBBITCOIN_ZMQ=libbitcoin_zmq.a
endif
if ENABLE_PROTON
LIBBITCOIN_PROTON=libbitcoin_proton.a
endif
if BUILD_BITCOIN_LIBS
LIBZCASH_CONSENSUS=libzcashconsensus.la
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

$(LIBUNIVALUE): $(wildcard univalue/lib/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# libcjson build
LIBCJSON=libcjson.a
libcjson_a_SOURCES = cJSON.c \
  komodo_cJSON.c komodo_cutils.cpp
libcjson_a_CPPFLAGS=-fPIC
EXTRA_LIBRARIES += $(LIBCJSON)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_COMMON) \
  $(LIBBITCOIN_SERVER) \
  $(LIBBITCOIN_CLI) \
  $(LIBZCASH) \
  $(LIBCC)

if ENABLE_WALLET
BITCOIN_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += $(LIBBITCOIN_WALLET)
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += $(LIBBITCOIN_ZMQ)
endif
if ENABLE_PROTON
EXTRA_LIBRARIES += $(LIBBITCOIN_PROTON)
endif

lib_LTLIBRARIES = $(LIBZCASH_CONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =

#if BUILD_BITCOIND
  bin_PROGRAMS += komodod
#endif

if BUILD_BITCOIN_UTILS
  bin_PROGRAMS += komodo-cli komodo-tx
endif
if ENABLE_WALLET
  bin_PROGRAMS += wallet-utility
endif


LIBZCASH_H = \
  zcash/IncrementalMerkleTree.hpp \
  zcash/NoteEncryption.hpp \
  zcash/Address.hpp \
  zcash/JoinSplit.hpp \
  zcash/Note.hpp \
  zcash/prf.h \
  zcash/Proof.hpp \
  zcash/util.h \
  zcash/Zcash.h \
  zcash/zip32.h

.PHONY: FORCE check-symbols check-security
# bitcoin core #
BITCOIN_CORE_H = \
  addressindex.h \
  spentindex.h \
  addrman.h \
  alert.h \
  amount.h \
  amqp/amqpabstractnotifier.h \
  amqp/amqpconfig.h \
  amqp/amqpnotificationinterface.h \
  amqp/amqppublishnotifier.h \
  amqp/amqpsender.h \
  arith_uint256.h \
  asyncrpcoperation.h \
  asyncrpcqueue.h \
  base58.h \
  bech32.h \
  bloom.h \
  cc/eval.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/params.h \
  consensus/upgrades.h \
  consensus/validation.h \
  core_io.h \
  core_memusage.h \
  deprecation.h \
  fs.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  key.h \
  key_io.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  metrics.h \
  miner.h \
  mruset.h \
  net.h \
  netbase.h \
  notaries_staked.h \
  noui.h \
  paymentdisclosure.h \
  paymentdisclosuredb.h \
  policy/fees.h \
  pow.h \
  prevector.h \
  primitives/block.h \
  primitives/transaction.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  rpc/client.h \
  rpc/protocol.h \
  rpc/server.h \
  rpc/register.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/serverchecker.h \
  script/sign.h \
  script/standard.h \
  serialize.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  transaction_builder.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  util/asmap.h \
  uint256.h \
  uint252.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet/asyncrpcoperation_mergetoaddress.h \
  wallet/asyncrpcoperation_sendmany.h \
  wallet/asyncrpcoperation_shieldcoinbase.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	$(abs_top_srcdir)
libbitcoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: zcashd
libbitcoin_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libbitcoin_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_server_a_CPPFLAGS += -fPIC
libbitcoin_server_a_SOURCES = \
  sendalert.cpp \
  addrman.cpp \
  alert.cpp \
  alertkeys.h \
  asyncrpcoperation.cpp \
  asyncrpcqueue.cpp \
  bloom.cpp \
  cc/eval.cpp \
  cc/import.cpp \
  cc/importgateway.cpp \
  cc/CCassetsCore.cpp \
  cc/CCcustom.cpp \
  cc/CCtx.cpp \
  cc/CCutils.cpp \
  cc/CCtokens.cpp \
  cc/assets.cpp \
  cc/faucet.cpp \
  cc/rewards.cpp \
  cc/dice.cpp \
  cc/lotto.cpp \
  cc/fsm.cpp \
  cc/heir.cpp \
  cc/oracles.cpp \
  cc/payments.cpp \
  cc/gateways.cpp \
  cc/channels.cpp \
  cc/auction.cpp \
  cc/betprotocol.cpp \
  chain.cpp \
  checkpoints.cpp \
  fs.cpp \
  crosschain.cpp \
  deprecation.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  metrics.h \
  miner.cpp \
  net.cpp \
  notaries_staked.cpp \
  noui.cpp \
  notarisationdb.cpp \
  paymentdisclosure.cpp \
  paymentdisclosuredb.cpp \
  policy/fees.cpp \
  pow.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/crosschain.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/serverchecker.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBZCASH_H)

if ENABLE_ZMQ
libbitcoin_zmq_a_CPPFLAGS = $(BITCOIN_INCLUDES) $(ZMQ_CFLAGS)
libbitcoin_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_zmq_a_SOURCES = \
	zmq/zmqabstractnotifier.cpp \
	zmq/zmqnotificationinterface.cpp \
	zmq/zmqpublishnotifier.cpp
endif

if ENABLE_PROTON
libbitcoin_proton_a_CPPFLAGS = $(BITCOIN_INCLUDES)
libbitcoin_proton_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_proton_a_SOURCES = \
  amqp/amqpabstractnotifier.cpp \
  amqp/amqpnotificationinterface.cpp \
  amqp/amqppublishnotifier.cpp
endif

# wallet: zcashd, but only linked when wallet enabled
libbitcoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_wallet_a_SOURCES = \
  utiltest.cpp \
  utiltest.h \
  zcbenchmarks.cpp \
  zcbenchmarks.h \
  wallet/asyncrpcoperation_mergetoaddress.cpp \
  wallet/asyncrpcoperation_sendmany.cpp \
  wallet/asyncrpcoperation_shieldcoinbase.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  paymentdisclosure.cpp \
  paymentdisclosuredb.cpp \
  transaction_builder.cpp \
  wallet/rpcdisclosure.cpp \
  wallet/rpcdump.cpp \
  cc/CCtokens.cpp \
  cc/CCassetsCore.cpp \
  cc/CCassetstx.cpp \
  cc/CCtx.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  zcash/zip32.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBZCASH_H)

# a static library for cryptoconditions
libcc_a_SOURCES = cc/cclib.cpp
libcc_a_CXXFLAGS = -DBUILD_CUSTOMCC -I../secp256k1/include -I../depends/$(shell echo `../depends/config.guess`/include) -I./univalue/include -I./cryptoconditions/include -I./cryptoconditions/src -I./cryptoconditions/src/asn -I. -I./cc
# libcc_a_LDFLAGS = -version-info 0:0:0

# crypto primitives library
crypto_libbitcoin_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_CONFIG_INCLUDES)
crypto_libbitcoin_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitcoin_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/equihash.cpp \
  crypto/equihash.h \
  crypto/equihash.tcc \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

if EXPERIMENTAL_ASM
  crypto_libbitcoin_crypto_a_SOURCES += crypto/sha256_sse4.cpp
endif

if ENABLE_MINING
EQUIHASH_TROMP_SOURCES = \
	pow/tromp/equi_miner.h \
	pow/tromp/equi.h \
	pow/tromp/osx_barrier.h
	
EQUIHASH_TROMP_AVX2_SOURCES = \
  pow/tromp/blake2-avx2/blake2b-common.h \
  pow/tromp/blake2-avx2/blake2b-load-avx2-simple.h \
  pow/tromp/blake2-avx2/blake2bip.c \
  pow/tromp/blake2-avx2/blake2bip.h

# crypto primitives library
crypto_libbitcoin_crypto_a_CPPFLAGS += $(BITCOIN_CONFIG_INCLUDES) -DEQUIHASH_TROMP_UNROLL

crypto_libbitcoin_crypto_a_SOURCES += \
	${EQUIHASH_TROMP_SOURCES}
if USE_TROMP_AVX2
    crypto_libbitcoin_crypto_a_CPPFLAGS += -mavx2
    crypto_libbitcoin_crypto_a_SOURCES += ${EQUIHASH_TROMP_AVX2_SOURCES}
else
    crypto_libbitcoin_crypto_a_CPPFLAGS += -DEQUIHASH_TROMP_ATOMIC
endif
endif

# common: shared between zcashd and non-server tools
libbitcoin_common_a_CPPFLAGS = -fPIC $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_common_a_CXXFLAGS = -fPIC $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  bech32.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/upgrades.cpp \
  core_read.cpp \
  core_write.cpp \
  hash.cpp \
  hex.c \
  importcoin.cpp \
  key.cpp \
  key_io.cpp \
  keystore.cpp \
  komodo_structs.cpp \
  komodo.cpp \
  komodo_bitcoind.cpp \
  komodo_ccdata.cpp \
  komodo_curve25519.cpp \
  komodo_events.cpp \
  komodo_gateway.cpp \
  komodo_globals.cpp \
  komodo_hardfork.cpp \
  komodo_interest.cpp \
  komodo_kv.cpp \
  komodo_notary.cpp \
  komodo_utils.cpp \
  netbase.cpp \
  metrics.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/cc.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_ext.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  transaction_builder.cpp \
  cc/CCtokenutils.cpp \
  cc/CCutilbits.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBZCASH_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libbitcoin_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  util/asmap.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBZCASH_H)

if GLIBC_BACK_COMPAT
libbitcoin_util_a_SOURCES += compat/glibc_compat.cpp
endif


# cli: zcash-cli
libbitcoin_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_cli_a_SOURCES = \
  rpc/client.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBZCASH_H)

nodist_libbitcoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# bitcoind binary #
komodod_SOURCES = bitcoind.cpp
komodod_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
komodod_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
komodod_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
komodod_SOURCES += bitcoind-res.rc
endif

komodod_LDADD = \
  $(LIBBITCOIN_SERVER) \
  $(LIBBITCOIN_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_ZMQ) \
  $(LIBBITCOIN_PROTON) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBZCASH) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1) \
  $(LIBCRYPTOCONDITIONS)

if ENABLE_WALLET
komodod_LDADD += $(LIBBITCOIN_WALLET)
endif

komodod_LDADD += \
  $(LIBCJSON) \
  $(BOOST_LIBS) \
  $(BDB_LIBS) \
  $(SSL_LIBS) \
  $(CRYPTO_LIBS) \
  $(EVENT_PTHREADS_LIBS) \
  $(EVENT_LIBS) \
  $(ZMQ_LIBS) \
  $(PROTON_LIBS) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBZCASH_LIBS) \
  $(LIBCC) \
  -lcurl

if ENABLE_PROTON
komodod_LDADD += $(LIBBITCOIN_PROTON) $(PROTON_LIBS)
endif

# bitcoin-cli binary #
komodo_cli_SOURCES = bitcoin-cli.cpp
komodo_cli_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES) $(EVENT_CFLAGS)
komodo_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
komodo_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

# wallet-utility binary #
if ENABLE_WALLET
wallet_utility_SOURCES = wallet-utility.cpp
wallet_utility_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
wallet_utility_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
wallet_utility_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
wallet_utility_SOURCES += wallet-utility-res.rc
endif
endif

if TARGET_WINDOWS
komodo_cli_SOURCES += bitcoin-cli-res.rc
endif

komodo_cli_LDADD = \
	$(LIBBITCOIN_CLI) \
	$(LIBUNIVALUE) \
	$(LIBBITCOIN_UTIL) \
	$(BOOST_LIBS) \
	$(SSL_LIBS) \
	$(CRYPTO_LIBS) \
	$(EVENT_LIBS) \
	$(LIBZCASH) \
	$(LIBBITCOIN_CRYPTO) \
  $(LIBZCASH_LIBS)

if ENABLE_WALLET
wallet_utility_LDADD = \
	libbitcoin_wallet.a \
	$(LIBBITCOIN_COMMON) \
	$(LIBBITCOIN_CRYPTO) \
	$(LIBSECP256K1) \
	$(LIBBITCOIN_UTIL) \
	$(BOOST_LIBS) \
	$(BDB_LIBS) \
	$(CRYPTO_LIBS) \
	$(LIBZCASH) \
	$(LIBZCASH_LIBS)\
	$(LIBCRYPTOCONDITIONS)
endif

# zcash-tx binary #
komodo_tx_SOURCES = komodo-tx.cpp
komodo_tx_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
komodo_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
komodo_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
komodo_tx_SOURCES += bitcoin-tx-res.rc
endif

# FIXME: Is libzcash needed for zcash_tx?
komodo_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBBITCOIN_COMMON) \
  $(LIBBITCOIN_UTIL) \
  $(LIBSECP256K1) \
  $(LIBZCASH) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBZCASH_LIBS) \
  $(LIBCRYPTOCONDITIONS)

komodo_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)

# zcash protocol primitives #
libzcash_a_SOURCES = \
	zcash/IncrementalMerkleTree.cpp \
	zcash/NoteEncryption.cpp \
	zcash/Address.cpp \
	zcash/JoinSplit.cpp \
	zcash/Proof.cpp \
	zcash/Note.cpp \
	zcash/prf.cpp \
	zcash/util.cpp \
	zcash/zip32.cpp

libzcash_a_CPPFLAGS = -fPIC -DBINARY_OUTPUT -DCURVE_ALT_BN128 \
	-DBOOST_SPIRIT_THREADSAFE -DHAVE_BUILD_INFO -D__STDC_FORMAT_MACROS $(HARDENED_CPPFLAGS) \
	$(HARDENED_CXXFLAGS) $(HARDENED_LDFLAGS) -pipe $(SAN_LDFLAGS) \
	-Wstack-protector $(SAN_CXXFLAGS) -fstack-protector-all -fPIE -fvisibility=hidden \
	-DSTATIC $(BITCOIN_INCLUDES)
libzcash_a_CXXFLAGS = $(SAN_CXXFLAGS) $(HARDENED_CXXFLAGS) -fwrapv -fno-strict-aliasing
libzcash_a_LDFLAGS = $(SAN_LDFLAGS) $(HARDENED_LDFLAGS)
libzcash_a_CPPFLAGS += -DMONTGOMERY_OUTPUT

# zcashconsensus library #
if BUILD_BITCOIN_LIBS
include_HEADERS = script/zcashconsensus.h
libzcashconsensus_la_SOURCES = \
  crypto/equihash.cpp \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  hash.cpp \
  primitives/transaction.cpp \
  pubkey.cpp \
  script/zcashconsensus.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
libzcashconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libzcashconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libzcashconsensus_la_LIBADD = $(LIBSECP256K1)
libzcashconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -I$(srcdir)/cryptoconditions/include -DBUILD_BITCOIN_INTERNAL
libzcashconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a *.gcda *.gcno */*.gcno wallet/*/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-$(RM) *.a
	-$(RM) crypto/*.a
	-$(RM) cryptoconditions/.libs/*.a

clean-all: clean-local
	-$(MAKE) -C univalue clean-all

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat of [$(bin_PROGRAMS)]..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security of [$(bin_PROGRAMS)]..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.ktest.include
#include Makefile.test.include # bitcoin tests
#include Makefile.gtest.include # zcash tests
endif
