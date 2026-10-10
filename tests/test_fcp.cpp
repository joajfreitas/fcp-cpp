#include <gtest/gtest.h>

#include "candata.h"
#include "fcp.hpp"
#include "fcp_device.hpp"
#include "fcp_message.hpp"

#include <optional>
#include <string>

using json = nlohmann::json;

/*
 * Test configuration constants.
 *
 * send_cmd signal layout:
 *   id:   bits  0-15
 *   dst:  bits 16-23
 *   arg1: bits 24-39
 *   arg2: bits 40-55
 *   arg3: bits 56-63
 */
static constexpr uint16_t INTERFACE_ID = 10;
static constexpr uint16_t MASTER_ID = 5;
static constexpr uint16_t CMD_ID = 3;
static constexpr uint16_t SEND_CMD_ID = 61;

/* Command arguments used by the tests */
static constexpr uint16_t CMD_ARG1 = 100;
static constexpr uint16_t CMD_ARG2 = 200;
static constexpr uint16_t CMD_ARG3 = 50;

static json make_signal(std::string name, int start, int length)
{
	return json{ { "name", name },
		{ "start", start },
		{ "length", length },
		{ "scale", 1 },
		{ "offset", 0 },
		{ "type", "unsigned" },
		{ "byte_order", "little_endian" },
		{ "mux", "" },
		{ "mux_count", 1 },
		{ "unit", "" },
		{ "comment", "" },
		{ "min_value", 0 },
		{ "max_value", 65535 } };
}

static json make_send_cmd_msg_json(uint16_t msg_id)
{
	return json{ { "name", "send_cmd" },
		{ "id", msg_id },
		{ "dlc", 8 },
		{ "frequency", 0 },
		{ "extended_frame", false },
		{ "signals",
			json{ { "id", make_signal("id", 0, 16) },
				{ "dst", make_signal("dst", 16, 8) },
				{ "arg1", make_signal("arg1", 24, 16) },
				{ "arg2", make_signal("arg2", 40, 16) },
				{ "arg3", make_signal("arg3", 56, 8) } } } };
}

static json make_device_json(std::string name, uint16_t id)
{
	return json{ { "name", name },
		{ "id", id },
		{ "comment", "test device" },
		{ "msgs", json::object() },
		{ "cmds", json::object() },
		{ "cfgs", json::object() } };
}

/*
 * Builds a valid FCP config. `send_cmd_id` is the message id given to the
 * common "send_cmd" message; 0 turns the message off entirely.
 */
static json make_config(uint16_t send_cmd_id)
{
	json devices;
	devices["interface"] = make_device_json("interface", INTERFACE_ID);

	json master = make_device_json("master", MASTER_ID);
	master["cmds"]["cmd_1"] = json{ { "name", "cmd_1" },
		{ "id", CMD_ID },
		{ "n_args", 3 },
		{ "comment", "test command" },
		{ "args", json::array() },
		{ "rets", json::array() } };
	devices["master"] = master;

	json common = json{ { "name", "common" },
		{ "id", 0 },
		{ "comment", "common device" },
		{ "msgs", json::object() },
		{ "cmds", json::object() },
		{ "cfgs", json::object() } };
	if (send_cmd_id > 0) {
		common["msgs"]["send_cmd"] = make_send_cmd_msg_json(send_cmd_id);
	}

	return json{ { "devices", devices },
		{ "common", common },
		{ "logs", json::object() } };
}

static json get_send_cmd_signal_map(double id, double dst, double arg1, double arg2, double arg3)
{
	return { { "id", id }, { "dst", dst }, { "arg1", arg1 },
		{ "arg2", arg2 }, { "arg3", arg3 } };
}

TEST(FcpEncodeCmd, SidUsesSendCmdMessageId)
{
	Fcp fcp;
	fcp.decompile(make_config(SEND_CMD_ID));

	std::optional<CanData> msg = fcp.encode_cmd("interface", "cmd_1",
		"master", CMD_ARG1, CMD_ARG2, CMD_ARG3);

	ASSERT_TRUE(msg.has_value());
	EXPECT_EQ(msg->get_sid(), (SEND_CMD_ID << 5) | INTERFACE_ID);
	EXPECT_EQ(get_msg_id(msg->get_sid()), (uint32_t)SEND_CMD_ID);
	EXPECT_EQ(get_dev_id(msg->get_sid()), (uint32_t)INTERFACE_ID);
	EXPECT_EQ(msg->get_dlc(), 8);
}

TEST(FcpEncodeCmd, SidFollowsConfiguredSendCmdId)
{
	Fcp fcp;
	fcp.decompile(make_config(63));

	std::optional<CanData> msg = fcp.encode_cmd("interface", "cmd_1",
		"master", CMD_ARG1, CMD_ARG2, CMD_ARG3);

	ASSERT_TRUE(msg.has_value());
	EXPECT_EQ(msg->get_sid(), (63 << 5) | INTERFACE_ID);
	EXPECT_EQ(get_msg_id(msg->get_sid()), (uint32_t)63);
	EXPECT_EQ(get_dev_id(msg->get_sid()), (uint32_t)INTERFACE_ID);
}

TEST(FcpEncodeCmd, PayloadCarriesCmdIdAndDstId)
{
	Fcp fcp;
	fcp.decompile(make_config(SEND_CMD_ID));

	std::optional<CanData> msg = fcp.encode_cmd("interface", "cmd_1",
		"master", CMD_ARG1, CMD_ARG2, CMD_ARG3);

	ASSERT_TRUE(msg.has_value());
	/* Signal layout:
	 * id: bits 0-15 | dst: bits 16-23 | arg1: bits 24-39
	 * arg2: bits 40-55 | arg3: bits 56-63                             */
	EXPECT_EQ(msg->get_data(0), (uint16_t)CMD_ID);
	EXPECT_EQ(msg->get_data(1),
		(uint16_t)(MASTER_ID | (CMD_ARG1 << 8)));
	EXPECT_EQ(msg->get_data(2),
		(uint16_t)((CMD_ARG1 >> 8) | ((CMD_ARG2 & 0xFF) << 8)));
	EXPECT_EQ(msg->get_data(3),
		(uint16_t)((CMD_ARG2 >> 8) | (CMD_ARG3 << 8)));
}

TEST(FcpEncodeCmd, FrameDecodesToSendCmdSignals)
{
	Fcp fcp;
	json config = make_config(SEND_CMD_ID);
	fcp.decompile(config);

	std::optional<CanData> msg = fcp.encode_cmd("interface", "cmd_1",
		"master", CMD_ARG1, CMD_ARG2, CMD_ARG3);
	ASSERT_TRUE(msg.has_value());

	FcpMessage send_cmd;
	send_cmd.decompile(config["common"]["msgs"]["send_cmd"]);

	DecodedMsg decoded = send_cmd.decode_msg(*msg);
	EXPECT_EQ(decoded.first, std::string("send_cmd"));
	ASSERT_EQ(decoded.second.count("id"), 1u);
	ASSERT_EQ(decoded.second.count("dst"), 1u);
	ASSERT_EQ(decoded.second.count("arg1"), 1u);
	ASSERT_EQ(decoded.second.count("arg2"), 1u);
	ASSERT_EQ(decoded.second.count("arg3"), 1u);
	EXPECT_EQ(decoded.second["id"], (double)CMD_ID);
	EXPECT_EQ(decoded.second["dst"], (double)MASTER_ID);
	EXPECT_EQ(decoded.second["arg1"], (double)CMD_ARG1);
	EXPECT_EQ(decoded.second["arg2"], (double)CMD_ARG2);
	EXPECT_EQ(decoded.second["arg3"], (double)CMD_ARG3);
}

TEST(FcpEncodeCmd, RejectsUnknownInputs)
{
	Fcp fcp;
	fcp.decompile(make_config(SEND_CMD_ID));

	EXPECT_FALSE(fcp.encode_cmd("unknown_dev", "cmd_1", "master", CMD_ARG1,
		CMD_ARG2, CMD_ARG3)
			.has_value());
	EXPECT_FALSE(fcp.encode_cmd("interface", "cmd_1", "unknown_dev",
		CMD_ARG1, CMD_ARG2, CMD_ARG3)
			.has_value());
	EXPECT_FALSE(fcp.encode_cmd("interface", "unknown_cmd", "master",
		CMD_ARG1, CMD_ARG2, CMD_ARG3)
			.has_value());
}

TEST(FcpEncodeCmd, RequiresSendCmdInCommon)
{
	Fcp fcp;
	fcp.decompile(make_config(0));

	EXPECT_FALSE(fcp.encode_cmd("interface", "cmd_1", "master", CMD_ARG1,
		CMD_ARG2, CMD_ARG3)
			.has_value());
}

TEST(FcpMessageEncodeCmd, UsesProvidedMsgId)
{
	json config = make_config(SEND_CMD_ID);

	FcpMessage send_cmd;
	send_cmd.decompile(config["common"]["msgs"]["send_cmd"]);

	const uint8_t dev_id = 7;
	const uint32_t msg_id = 63;
	CanData msg = send_cmd.encode_cmd(get_send_cmd_signal_map(CMD_ID,
		MASTER_ID, CMD_ARG1, CMD_ARG2, CMD_ARG3), dev_id, msg_id);

	EXPECT_EQ(get_msg_id(msg.get_sid()), msg_id);
	EXPECT_EQ(get_dev_id(msg.get_sid()), (uint32_t)dev_id);

	DecodedMsg decoded = send_cmd.decode_msg(msg);
	EXPECT_EQ(decoded.first, std::string("send_cmd"));
	EXPECT_EQ(decoded.second["id"], (double)CMD_ID);
	EXPECT_EQ(decoded.second["dst"], (double)MASTER_ID);
	EXPECT_EQ(decoded.second["arg1"], (double)CMD_ARG1);
	EXPECT_EQ(decoded.second["arg2"], (double)CMD_ARG2);
	EXPECT_EQ(decoded.second["arg3"], (double)CMD_ARG3);
}
