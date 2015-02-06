#!usr/bin/perl
my $prj = $ARGV[0];
my $cfg = "mediatek/custom/$prj/security/image_encrypt/IMG_ENCRYPT_CFG.ini";
my $key = "mediatek/custom/$prj/security/image_encrypt/IMG_ENCRYPT_KEY.ini";


################################################################
# ENCRYPT PLAINTEXT IMAGE
################################################################
print "\n********************************************\n";
print " PLAINTEXT IMAGE \n";
print "********************************************\n\n\n";

my $dir = "out/target/product/$prj";

`mkdir $dir/enc_bin` if ( ! -d "$dir/enc_bin" );
print "Image Dir '$dir'\n";
my $command = "mediatek/build/tools/CipherTool/CipherTool";

my @imgs_need_enc = ("boot.img", "logo.bin", "recovery.img", "secro.img", "system.img", "uboot_${prj}.bin", "userdata.img");
foreach my $img (@imgs_need_enc) {
	push (@miss_img, $img) if ( ! -e "$dir/$img");
}
die "@miss_img\nall the imgs above is NOT exsit\n" if (@miss_img > 0);

foreach my $img (@imgs_need_enc) {
	if ( ! -e "$dir/$img") {
		warn "the $img is NOT exsit, please check\n";
		next;
	}
	my $enc_img = $img;
	$enc_img =~ s/\./-enc\./;
	print "Encrypt Image '$dir/$img' ...\n";
	&print_system("$command ENC $key $cfg $dir/$img $dir/enc_bin/$enc_img");
}

sub print_system {
	my $command = $_[0];
	my $rslt = system($command);
	print "$command: $rslt\n";
	die "Failed to execute $command" if ($rslt != 0);
}

################################################################
# ENCRYPT SIGNED IMAGE IF NEEDED
################################################################
print "\n\n\n********************************************\n";
print " SIGNED IMAGE\n";
print "********************************************\n\n\n";
my $dir = "out/target/product/$prj/signed_bin";

`mkdir $dir/enc_bin` if ( ! -d "$dir/enc_bin" );
print "Image Dir '$dir'\n";
my $command = "mediatek/build/tools/CipherTool/CipherTool";

my @imgs_need_enc = ("boot-sign.img", "logo-sign.bin", "recovery-sign.img", "secro-sign.img", "system-sign.img", "uboot_${prj}-sign.bin", "userdata-sign.img");
foreach my $img (@imgs_need_encrypt) {
	push (@miss_img, $img) if ( ! -e "$dir/$img");
}
die "@miss_img\nall the imgs above is NOT exsit\n" if (@miss_img > 0);

foreach my $img (@imgs_need_enc) {
	if ( ! -e "$dir/$img") {
		warn "the $img is NOT exsit, please check\n";
		next;
	}
	my $enc_img = $img;
	$enc_img =~ s/\./-enc\./;
	print "Encrypt Image '$dir/$img' ...\n";
	&print_system("$command ENC_SIGNED_BIN $key $cfg $dir/$img $dir/enc_bin/$enc_img");
}

sub print_system {
	my $command = $_[0];
	my $rslt = system($command);
	print "$command: $rslt\n";
	die "Failed to execute $command" if ($rslt != 0);
}
