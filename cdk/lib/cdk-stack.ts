import * as cdk from "aws-cdk-lib";
import { Construct } from "constructs";
import * as lambda from "aws-cdk-lib/aws-lambda";
import * as s3 from "aws-cdk-lib/aws-s3";
import * as ec2 from "aws-cdk-lib/aws-ec2";
import * as dynamodb from "aws-cdk-lib/aws-dynamodb";
import * as keypair from "cdk-ec2-key-pair";
import * as iam from "aws-cdk-lib/aws-iam";

const SERVICE_NAME = "lambdadb";

export class CdkStack extends cdk.Stack {
  constructor(scope: Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props);

    // Create S3 bucket
    const bucket = new s3.Bucket(this, `${SERVICE_NAME}-data-bucket`, {
      bucketName: `${SERVICE_NAME}-data-bucket`,
    });

    // Create deployment bucket
    const deploymentBucket = new s3.Bucket(this, `${SERVICE_NAME}-deployment-bucket`, {
      bucketName: `${SERVICE_NAME}-deployment-bucket`,
      publicReadAccess: true,
    });

    const functionIamRole = new iam.Role(this, `${SERVICE_NAME}-lambda-role`, {
      assumedBy: new iam.ServicePrincipal("lambda.amazonaws.com"),
    });

    // Add S3 permissions to lambda functions
    functionIamRole.addToPolicy(
      new iam.PolicyStatement({
        effect: iam.Effect.ALLOW,
        actions: ["s3:*"],
        resources: [bucket.bucketArn, `${bucket.bucketArn}/*`],
      })
    );

    // Create lambda functions with different memory sizes
    const memorySizes = [128, 256, 512, 1024, 2048, 3008];
    let functions = [];

    for (const memorySize of memorySizes) {
      functions.push(
        new lambda.Function(
          this,
          `${SERVICE_NAME}-func-${memorySize}`,
          {
            functionName: `${SERVICE_NAME}-func-${memorySize}`,
            code: lambda.Code.fromAsset("./runtimes/lambdadb.zip"),
            handler: "main_handler",
            runtime: lambda.Runtime.PROVIDED,
            memorySize,
            timeout: cdk.Duration.seconds(30),
            role: functionIamRole,
          }
        )
      );
    }

    // Create instance exploration dynamodb table
    // EC2 instances will register themselves here
    const instanceTable = new dynamodb.Table(
      this,
      `${SERVICE_NAME}-ec2-instance-table`,
      {
        partitionKey: {
          name: "instanceId",
          type: dynamodb.AttributeType.STRING,
        },
        timeToLiveAttribute: "ttl",
      }
    );

    // Create vpc
    const vpc = new ec2.Vpc(this, "vpc");

    // Create ec2 instances
    let instances = [];

    for (let i = 0; i < 1; i++) {
      // Create a key pair to be used with this EC2 Instance
      const key = new keypair.KeyPair(this, `${SERVICE_NAME}-keypair-${i}`, {
        name: `${SERVICE_NAME}-keypair-${i}`,
        description: "Key Pair created with CDK Deployment",
      });
      key.grantReadOnPublicKey;

      // Security group for the EC2 instance
      const securityGroup = new ec2.SecurityGroup(this, `${SERVICE_NAME}-ec2-sg-${i}`, {
        vpc,
        description: "Allow SSH (TCP port 22) and HTTP (TCP port 80) in",
        allowAllOutbound: true,
      });

      // Allow SSH access on port tcp/22
      securityGroup.addIngressRule(
        ec2.Peer.anyIpv4(),
        ec2.Port.tcp(22),
        "Allow SSH Access"
      );

      // Allow TCP access on port tcp/4789
      securityGroup.addIngressRule(
        ec2.Peer.anyIpv4(),
        ec2.Port.tcp(4789),
        "Allow TCP/LambdaDB Access"
      );

      // IAM role to allow access to other AWS services
      const role = new iam.Role(this, `${SERVICE_NAME}-ec2-role-${i}`, {
        assumedBy: new iam.ServicePrincipal("ec2.amazonaws.com"),
      });

      // IAM policy attachment to allow access to
      role.addManagedPolicy(
        iam.ManagedPolicy.fromAwsManagedPolicyName(
          "AmazonSSMManagedInstanceCore"
        )
      );

      // Look up the AMI Id for the Amazon Linux 2 Image with CPU Type X86_64
      const ami = new ec2.AmazonLinuxImage({
        generation: ec2.AmazonLinuxGeneration.AMAZON_LINUX_2,
        cpuType: ec2.AmazonLinuxCpuType.X86_64,
      });

      // Create the EC2 instance using the Security Group, AMI, and KeyPair defined.
      instances.push(
        new ec2.Instance(this, `ec2-instance-${i}`, {
          vpc,
          instanceType: ec2.InstanceType.of(
            ec2.InstanceClass.T3,
            ec2.InstanceSize.MEDIUM
          ),
          machineImage: ami,
          securityGroup: securityGroup,
          keyName: key.keyPairName,
          role: role,
          vpcSubnets: {
            subnetType: cdk.aws_ec2.SubnetType.PUBLIC,
          }
        })
      );
    }

    // Add S3 permissions to ec2 instances
    for (const instance of instances) {
      bucket.grantRead(instance);
      bucket.grantWrite(instance);
      bucket.grantDelete(instance);
      bucket.grantPut(instance);
    }

    // Add dynamodb permissions to ec2 instances
    for (const instance of instances) {
      instanceTable.grantReadWriteData(instance);
    }

    // Create codebuild with github source
    /*const codebuildProject = new codebuild.PipelineProject(
      this,
      `${SERVICE_NAME}-codebuild-project`,
      {
        environment: {
          buildImage: codebuild.LinuxBuildImage.AMAZON_LINUX_2_4,
        },
        cache: codebuild.Cache.bucket(
          new s3.Bucket(this, `${SERVICE_NAME}-codebuild-cache`)
        ),
        buildSpec: codebuild.BuildSpec.fromObject({
          version: "0.2",
          phases: {
            install: {
              commands: [
                "echo Entered the install phase...",
                "echo Installing cmake 3.25...",
                "mkdir -p cmake-3.25 && wget -qO- "https://cmake.org/files/v3.25/cmake-3.25.0-linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C cmake-3.25",
                "export PATH=`pwd`/cmake-3.25/bin:$PATH",
                "cmake --version",
                "echo Installing the AWS SDK for C++...",
                "git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp",
                "mkdir -p aws-sdk-cpp/build && cd aws-sdk-cpp/build",
                "cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/local/ -DCMAKE_INSTALL_PREFIX=/usr/local/ -DBUILD_ONLY=\"s3\"",
                "make",
                "make install",
                "cd ../..",
                "echo Installing the AWS Lambda Runtime...",
                "git clone https://github.com/awslabs/aws-lambda-cpp.git",
                "cd aws-lambda-cpp",
                "mkdir build && cd build",
                "cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local/",
                "make",
                "make install",
                "cd ../..",
                "echo Installing libc...",
                "yum install glibc-devel",
                "yum install glibc-devel.i686"
              ],
            },
            build: {
              commands: [
                "gcc --version",
                "echo Building Lambda package...",
                "mkdir build-lambda && cd build-lambda",
                "cmake .. -DCMAKE_BUILD_TYPE=Release -DRUNTIME=AWS_LAMBDA",
                "make",
                "make aws-lambda-package-lambdadb",
                "echo Copying lambdadb.zip to S3...",
                `aws s3 cp lambdadb.zip s3://${deploymentBucket.bucketName}/lambdadb-lambda.zip`,
                "cd ..",
                "echo Building EC2 package...",
                "mkdir build-ec2 && cd build-ec2",
                "cmake .. -DCMAKE_BUILD_TYPE=Release -DRUNTIME=AWS_EC2",
                "make",
                "echo Copying lambdadb-ec2 to S3...",
                `aws s3 cp lambdadb.zip s3://${deploymentBucket.bucketName}/lambdadb-ec2.zip`,
                "cd ..",
              ],
            },
            post_build: {
              commands: [
                "echo Entered the post_build phase...",
                "echo Updating lambda functions",
                ...functions.map(
                  (f) =>
                    `aws lambda update-function-code --function-name ${f.functionName} --zip-file fileb://build-lambda/lambdadb.zip`
                ),
                "echo Done updating lambda functions",
                "echo Updating ec2 instances",
                ...instances.map(
                  (i) =>
                    `aws ssm send-command --document-name "AWS-RunShellScript" --parameters commands="pkill -f lambdadb-ec2 && curl -O https://s3.amazonaws.com/${deploymentBucket.bucketName}/lambdadb-ec2 && ./lambdadb-ec2" --instance-ids ${i.instanceId} --output text --region ${this.region}`
                ),
                "echo Done updating ec2 instances",
              ],
            },
          },
        }),
      }
    );

    codebuildProject.addToRolePolicy(
      new iam.PolicyStatement({
        resources: functions.map((f) => f.functionArn),
        actions: ["lambda:UpdateFunctionCode"],
      })
    );

    codebuildProject.addToRolePolicy(
      new iam.PolicyStatement({
        resources: [deploymentBucket.bucketArn, `${deploymentBucket.bucketArn}/*`],
        actions: ["s3:GetObject", "s3:PutObject"],
      })
    );

    // Create codepipeline
    const pipeline = new codepipeline.Pipeline(
      this,
      `${SERVICE_NAME}-codepipeline`,
      {
        pipelineName: `${SERVICE_NAME}-codepipeline`,
      }
    );

    // Add source stage
    const sourceOutput = new codepipeline.Artifact();
    const sourceAction = new codepipeline_actions.GitHubSourceAction({
      actionName: "GitHub",
      owner: "MaxiMittel",
      repo: "lambdadb",
      oauthToken: cdk.SecretValue.secretsManager("github-token"),
      output: sourceOutput,
      branch: "master",
    });

    pipeline.addStage({
      stageName: "Source",
      actions: [sourceAction],
    });

    // Add build stage
    const buildOutput = new codepipeline.Artifact();
    const buildAction = new codepipeline_actions.CodeBuildAction({
      actionName: "CodeBuild",
      project: codebuildProject,
      input: sourceOutput,
      outputs: [buildOutput],
    });

    pipeline.addStage({
      stageName: "Build",
      actions: [buildAction],
    });*/

    // Create outputs
    new cdk.CfnOutput(this, "functions", {
      value: functions.map((f) => f.functionName).join("\n"),
    });

    // Output the public IP address of the EC2 instance
    new cdk.CfnOutput(this, "IP Address", {
      value: instances.map((i) => i.instancePublicIp).join("\n"),
    });

    // Output the instance ID of the EC2 instance
    new cdk.CfnOutput(this, "Instance ID", {
      value: instances.map((i) => i.instanceId).join("\n"),
    });

    // Command to download the SSH key
    new cdk.CfnOutput(this, "Download Key Command", {
      value: instances
        .map(
          (i) =>
            `aws secretsmanager get-secret-value --secret-id ec2-ssh-key/${SERVICE_NAME}-keypair-${i}/private --query SecretString --output text > cdk-key.pem && chmod 400 cdk-key.pem`
        )
        .join("\n"),
    });

    // Command to access the EC2 instance using SSH
    new cdk.CfnOutput(this, "ssh command", {
      value: instances
        .map(
          (i) =>
            `ssh -i cdk-key.pem -o IdentitiesOnly=yes ec2-user@${i.instancePublicIp}`
        )
        .join("\n"),
    });

    new cdk.CfnOutput(this, "bucket", {
      value: bucket.bucketName,
    });

    new cdk.CfnOutput(this, "deploymentBucket", {
      value: deploymentBucket.bucketName,
    });

    new cdk.CfnOutput(this, "instanceTable", {
      value: instanceTable.tableName,
    });
  }
}
