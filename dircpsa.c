#include <stdio.h>
#include <string.h>
#include <libircclient.h>

typedef struct {
	char *channel;
	char *message;
} irc_ctx_t;

void event_connect(irc_session_t *session, const char *event, const char *origin
		, const char **params, unsigned int count)
{
	irc_ctx_t *ctx = (irc_ctx_t *)irc_get_ctx(session);
	irc_cmd_join(session, ctx->channel, NULL);
}

void event_join(irc_session_t *session, const char *event, const char *origin
		, const char **params, unsigned int count)
{
	irc_ctx_t *ctx = (irc_ctx_t *)irc_get_ctx(session);
	irc_cmd_user_mode(session, "+i");
	irc_cmd_msg(session, params[0], ctx->message);
	irc_cmd_quit (session, "selesai");
}

int main (int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s <channel> <message>\n", argv[0]);
		return 1;
	}
	irc_ctx_t ctx = {.channel = argv[1], .message = argv[2]};

	irc_callbacks_t	callbacks;
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.event_connect = event_connect;
	callbacks.event_join = event_join;

	irc_session_t *session = irc_create_session(&callbacks);
	if (!session) {
		printf("Could not create session\n");
		return 1;
	}
	irc_set_ctx(session, &ctx);

	if (irc_connect(session, "irc.darapsa.co.id", 6667, 0, "git", "interch"
				, "Darapsa")) {
		printf("Could not connect: %s\n"
				, irc_strerror(irc_errno(session)));
		return 1;
	}

	if (irc_run(session)) {
		printf ("Could not connect or I/O error: %s\n"
				, irc_strerror (irc_errno(session)));
		return 1;
	}

	return 1;
}