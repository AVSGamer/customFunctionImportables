param (
    [Parameter(Mandatory=$true)]
    [string]$CommitMessage
)

$status = git status --porcelain
if ($status) {
    Write-Host "?? Local changes detected. Stage, commit, and sync your working tree first." -ForegroundColor Yellow
}

Write-Host "?? Fetching existing tags from origin remote..." -ForegroundColor Cyan
git fetch --tags

$lastTag = git describe --tags --abbrev=0 2>$null
if (-not $lastTag) {
    $lastTag = "v1.0.0"
    Write-Host "?? No version tags discovered in repository history. Initializing base: v1.0.0" -ForegroundColor Blue
}

if ($lastTag.StartsWith("v")) {
    $cleanTag = $lastTag.Substring(1)
    $parts = $cleanTag.Split('.')
    if ($parts.Count -eq 3) {
        $major = [int]$parts[0]
        $minor = [int]$parts[1]
        $patch = [int]$parts[2]
        $patch++
        $nextTag = "v{0}.{1}.{2}" -f $major, $minor, $patch
    } else {
        $nextTag = "v1.0.1"
    }
} else {
    $nextTag = "v1.0.1"
}

$moveMsg = "?? Moving version target forward: {0} -> {1}" -f $lastTag, $nextTag
Write-Host $moveMsg -ForegroundColor Green

$commitMsgFull = "{0} (Release {1})" -f $CommitMessage, $nextTag
Write-Host "?? Committing project file workspace updates..." -ForegroundColor Blue
git add .
git commit -m $commitMsgFull

Write-Host "??? Applying production version release tag..." -ForegroundColor Blue
git tag $nextTag

Write-Host "??? Pushing codebase updates upstream to remote origin server..." -ForegroundColor Cyan
git push origin main
git push origin $nextTag

Write-Host "? Deployment initialization complete! Your compilation pipelines are running." -ForegroundColor Green
