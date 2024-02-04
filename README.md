Minishell was the first group project, but also the first HUGE project of 42-CommonCore. The basic idea was to **reproduce a leightweight shell using Bash as a reference**. To quote the subject's introduction, which I couldn't have phrased any better:

> *“The existence of shells is linked to the very existence of IT.
At the time, all developers agreed that communicating with a computer using aligned 1/0 switches was seriously irritating.
It was only logical that they came up with the idea of creating a software to communicate with a computer using interactive lines of commands in a language somewhat close to the human language.
Thanks to Minishell, you’ll be able to travel through time and come back to problems people faced when Windows didn’t exist.”*
> 

This project took some sleepless nights, and a lot of determination. When coding a shell, even a light shell, **you have to think about EVERYTHING**.

Among the things the subject asked us to reproduce, the standouts of this Minishell were :

### Parsing, especially quotes

Parsing means taking elements passed on the command line and transforming them into tokens that can be used as arguments (to an execve, for example). Although the subject didn't require us to interpret unclosed quotes (we were content to return an error in this case), parsing quotes required a good deal of logic and precision. On a test like this, for example:

```markdown
> echo s"'""'sa""a"'"'"'"l"lu''t'"''t"''"'"'"'"
```

We had to make sure that the minishell returned the same thing as Bash, i.e. :

```markdown
s''saa"'llu''t't''"'
```

### Expands and environment variables

Expands are the little **'$'** character placed in front of a sequence of alphanumeric values (+ underscore) or a **'?'**, which allows you to expand the designated variable. In the first case, a :

```markdown
> echo $USER
```

calls up the **environment variable** contained by "USER", i.e. the user name (if it hasn't been modified beforehand by a little joker...). In the same spirit, a :

```markdown
> echo $?
```

should display the **output code of the last command executed**. For example, if I've just run an 'ls' and all went well, 'echo $?' should display '0'.

Expands get a bit tricky when it comes to retrieving multi-part **environment variables**. For example, if you have an environment variable like this :

```markdown
TEST=hello my friends
```

we have to make sure that our token creation functions **returned to the variable to retrieve all its parts.**

We also had to be careful to manage our tokens properly, as this too required a great deal of precision. If we were to send our shell something like this :

```markdown
> export A="l"
> export B="s -la"
> $A$B
```

you guessed it, the shell will have to execute an "ls -la".

### Pipes

The '|' pipe is a tool used to **connect the output of one command to the input of the next**. Here again, good precision was needed to manage fds correctly (and even more so if you were to have fun introducing redirections...). A minishell in which pipes are well managed should be able to handle commands like these:

```markdown
> ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls | ls
includes libft srcs Makefile
```

Or to exit a cat | ls in as many \n as there are cat in the command:

```markdown
> cat | cat | cat | ls



>
```

This, by the way, is a very good indicator of pipeline quality, because if the cat's don't come out in succession when they receive their \n, it means they're still waiting to receive something from the pipe, which means that a fd is poorly closed.

### Redirections

Just like pipes, redirections require good fds management to correctly redirect inputs (**'<'**) or outputs (**'>'** and **'>>'**). We also had to manage **'<<'**, which opens a **here-document** (a temporary file that takes the place of the standard input, retrieves the inputs and is unlinked as soon as the command is executed):

```markdown
> << END << FIN cat
> hello
> my
> friends
> END
> hey
> $USER
> FIN
hey [expanded username]
```

There are a LOT of (more or less) amusing easter eggs with redirections. For example, :

```markdown
> echo hello > file1 > file2 > file3 > file4
```

creates each file, but only writes to the last (in this case file4). Or :

```markdown
> echo hello > file1 a > file2 b > file3 c > file4
```

will write "hello a b c" in file4, with the little elements hanging around (bros are just chillin) after filenames treated as echo arguments. We also have this funfact:

```markdown
> echo hello >> file1 > file2 >> file3 > file4
```

We know that "hello" will only be written to file4, but we also know that if the files already exist and have content, '>' will overwrite this content (the file will become blank), but not '>>'. In this case, the contents of file2 will have been completely deleted, file1 and file3 will have kept theirs, and the contents of file4 will have been replaced by "hello".

And there's so much more, I'm not even going to get into ambigous redirects, as that would be giving these redirects far more attention than they deserve 🙂. But here we are, now we can have fun looking at whether a WTF command such as :

```markdown
> cat < file1 > file2 >> file3 < file4 > file2 > file5 < file6 < file7 >> file1 > file4 < file3 >> file2 >> file6
```

gives us the same result as in bash. 🙂

### Builtins

Like Bash, our shell also had to integrate its own commands, i.e. some commands are not executed via their path and execve, but are called with our own code. Our builtins were :

- **'echo'**, with its **n** option to suppress line breaks;
- ‘cd', which had to be able to handle both relative and absolute paths. **'cd ~**' was to take us back to /home, and '**cd -**' was to take us to the path of the penultimate 'cd' made;
- **‘pwd'**, which prints the current working directory;
- **‘unset’** deletes an environment variable;
- **‘env'** prints our environment;
- **‘exit'** exits the shell with 0 if called with no argument, or with the specified numerical argument (modulo 256).
- and last but not least, **'export'**, which should be able to handle the addition or modification of an environment variable, or print the env if called without argument.

I'd say **'cd'** and **'export'** were the most important, because they forced you to think about a lot of stuff. ‘Export' was really tricky because it also required a lot of parsing.

Finally, we also had to handle signals such as **'CTRL+C'**, **'CTRL+D'** and **'CTRL+ \ '**, but other than that, that's about it. We didn't manage the bonus part, which involved dealing with operators such as ‘**||’**, ‘**&&’** and wildcards ‘*’, as we found the minishell challenging enough for what it was, and didn't really have any pedagogical interest in going that far.

If you'd like to try your hand at cracking our shell, you can retrieve it by doing :

```markdown
git clone https://github.com/rmnina/minishell.git
cd minishell
make && make clean
./minishell
```

Be nice though… 🙂
