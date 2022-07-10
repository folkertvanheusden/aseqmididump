#include <stdio.h>
#include <time.h>
#include <alsa/asoundlib.h>

uint64_t get_us()
{
        struct timespec tp { 0 };

        if (clock_gettime(CLOCK_REALTIME, &tp) == -1) {
                perror("clock_gettime");
                return 0;
        }

        return tp.tv_sec * 1000l * 1000l + tp.tv_nsec / 1000;
}

int main(int argc, char *argv[])
{
	snd_seq_t *seq_handle = nullptr;
	snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0);

	snd_seq_set_client_name(seq_handle, "MIDI dumper");
	snd_seq_create_simple_port(seq_handle, "in", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION);

	for(;;) {
		snd_seq_event_t *ev = nullptr;
		snd_seq_event_input(seq_handle, &ev);

		uint64_t when = get_us();

		if (ev->type == SND_SEQ_EVENT_NOTEON || ev->type == SND_SEQ_EVENT_NOTEOFF) {
			time_t t = when / 1000000;
			struct tm *tm = localtime(&t);

			printf("%lu (%04d:%02d:%02d %02d:%02d:%02d.%06lu) [%d] Note %s: %2x vel(%2x)\n",
				when, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, when % 1000000,
				ev->time.tick,
				ev->type == SND_SEQ_EVENT_NOTEON ? "on " : "off",
				ev->data.note.note,
				ev->data.note.velocity);
		}
	}

	return 0;
}
