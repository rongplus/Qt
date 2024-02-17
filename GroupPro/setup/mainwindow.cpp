#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QProcess>
#include <unistd.h>
 #include <iostream>
 #include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "df.h"

#define MAILPROG	"/usr/sbin/sendmail"
using namespace std;

#define PIPE_WRITE(fd, msg) { write(fd, msg, strlen(msg)); }

static struct safe_pid {
	struct safe_pid *next;
	int	fd;
	pid_t	pid;
} *pidlist = 0;


/*****************************************************************************
** SAFE_PCLOSE
*****************************************************************************/
int safe_pclose(int fd)
{
	struct safe_pid *cur, *last;
	pid_t pid;
	int pstat;

	for (last = NULL, cur = pidlist; cur; last = cur, cur = cur->next)
		if (cur->fd == fd) break;

	if (cur == NULL)
		return -1;

	close(cur->fd);

	do {
		pid = waitpid(cur->pid, &pstat, 0);
	} while (pid == -1 && errno == EINTR);

	if (last == NULL)
		pidlist = cur->next;
	else
		last->next = cur->next;

	free(cur);
	return (pid == -1 ? -1 : WEXITSTATUS(pstat));
}


/*****************************************************************************
** SAFE_POPEN
*****************************************************************************/
int safe_popen(const char *prog, const char *user, int study,
	const char *working_dir, const char *dir, const char *pass)
{
	struct safe_pid * volatile cur;
	struct rlimit fd_limit;
	pid_t	pid;
	unsigned int	fd, envc;
	char	*env[100+4], **e, *m, *v;
	char	*b, *c, *cmd = NULL;
	int pdes[2];
	int in_quote = 0;

	if (prog && *prog) { cmd = strdup(prog); }

	if (cmd == NULL) { return -1; }


	if (getrlimit(RLIMIT_NOFILE, &fd_limit) < 0) {
		free(cmd);
		return -1;
	}

	if ((cur = (safe_pid*)malloc(sizeof(struct safe_pid))) == NULL) {
		free(cmd);
		return -1;
	}

	if (pipe(pdes) < 0) {
		free(cmd);
		free(cur);
		return -1;
	}

	switch(pid = fork()) {
		/* Failure */
	case -1:
		close(pdes[0]);
		close(pdes[1]);
		free(cmd);
		free(cur);
		return -1;

		/* Child */
	case 0:
		/* Change working directory if specified */
		if (working_dir) {
			if (chdir(working_dir)) {
				free(cmd);
				return(127);
			}
		}
		/* Set up the environment */



		if (*dir == 'r') {
			close(STDIN_FILENO);
			open("/dev/null", O_RDONLY);
			dup2(pdes[1], STDOUT_FILENO);
			dup2(pdes[1], STDERR_FILENO);
		} else {
			dup2(pdes[0], STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			open("/dev/null", O_WRONLY);
			open("/dev/null", O_WRONLY);
		}
		for (fd = 3; fd < fd_limit.rlim_cur; fd++) close(fd);
		execle("/bin/sh", "sh", "-c", cmd, (char *)NULL, env);
		exit(127);
	}

	free(cmd);
	if (*dir == 'r') {
		close(pdes[1]);
		cur->fd = pdes[0];
	} else {
		close(pdes[0]);
		cur->fd = pdes[1];
	}
	cur->pid = pid;
	cur->next = pidlist;
	pidlist = cur;
	return cur->fd;
}


int rpc_mail_handler(char *address, char *replyto, char *subject, char *body)
{
	int fd;
	char *cmd;

	if (!address || !subject || !body || !strlen(address) ||
		!strlen(subject) || !strlen(body)) return 0;

	cmd = (char*)malloc(strlen(MAILPROG) + 16);
	sprintf(cmd, "%s -bm -t", MAILPROG);
	if ((fd = safe_popen(cmd, NULL, 0, NULL, "w", 0)) < 0) {
		free(cmd);
		cout << "error open sendmail" << endl;
		return 0;
	}
	free(cmd);

	PIPE_WRITE(fd, "To: ");
	PIPE_WRITE(fd, address);
	PIPE_WRITE(fd, "\nSubject: ");
	PIPE_WRITE(fd, subject);

	PIPE_WRITE(fd, "\nMime-Version: 1.0\n");
	PIPE_WRITE(fd, "Content-Type: text/html\n");
	if (replyto && strlen(replyto)) {
		PIPE_WRITE(fd, "\nReply-To: ");
		PIPE_WRITE(fd, replyto);
	}
	PIPE_WRITE(fd, "\n\n");
	PIPE_WRITE(fd, body);

	safe_pclose(fd);

	return 1;
}


int sendmail(const char *to, const char *from, const char *subject, const char *message)
{
	int retval = -1;
	FILE *mailpipe = popen("/usr/sbin/sendmail -t", "w");
	if (mailpipe != NULL)
	{
		const char* str = "Mime-Version: 1.0\n\
Content-Type: text/html\n\
<html>\
<body>\
<b>Does it scale you??.</b>\
<li>list a</li>\
<li>list b</li>\
<li>list c</li>\
<li>list d</li> \
</body>\
</html>";


		fprintf(mailpipe, "To: %s\n", to);
		fprintf(mailpipe, "From: %s\n" ,from);
		fprintf(mailpipe, "Reply-To: %s\n" ,from);
		fprintf(mailpipe, "Subject: %s\n", subject);
		fprintf(mailpipe, "Mime-Version: 1.0\n");
		fprintf(mailpipe, "Content-Type: text/html\n");

		fwrite(message, 1, strlen(message), mailpipe);
		fwrite(".\n", 1, 2, mailpipe);
		pclose(mailpipe);
		retval = 0;
	 }
	 else
	 {
		// perror("Failed to invoke sendmail”);
	 }
	 return retval;
}


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	int www = QApplication::desktop()->screen()->width();
	int hhh = QApplication::desktop()->screen()->height();
	//resize(www,hhh);
	//ui->scrollArea->setWidget(ui->img);
	QLabel* p = new QLabel(this);
	p->setFixedWidth(1000);
	p->setFixedHeight(1000);
	p->setPixmap( QPixmap(":/Json/abc.png"));
	ui->scrollArea->setWidget(p);
	ui->scrollArea->setWidgetResizable(false);

	QString mf = "rong@datafax.com";
	QString mt = "rong@datafax.com";
	QString ti = "test";
	QString ac = "";
	QString err = "a?";
	//sendEmail(mf,mt,ti,ac,err);
	 QProcess mail;
	 QString str = "From: me@example.org\
			 To: rong@datafax.com\
			 Subject: MIME Test\
			 Mime-Version: 1.0\
			 Content-Type: text/html\
			 <html>\
			 <body>\
			 This is a test.\
			 </body>\
			 </html>";
	 mail.start(MAILPROG, QStringList() <<"-t" << str << "\n")  ;
	std::cout << "d " << endl;
	 mail.close();
/*	 sendmail
	// rpc_mail_handler
("rong@datafax.com","hyppuma@163.com","让你骚！！","<html>\
		 <body>\
		 <b>Are You 18??.</b>\
		<li>一股骚气</li>\
<li>滚粗</li>\
<li>一股骚气</li>\
<li>滚粗</li>\
		 </body>\
		 </html>");

*/

	 DF *df = new DF(this);
	 df->show();
}

MainWindow::~MainWindow()
{
	delete ui;
}
